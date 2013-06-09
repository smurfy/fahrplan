#include "fahrplan_calendar_manager.h"

#include <QSettings>
#include <QtConcurrentRun>
#include <QFutureWatcher>

#ifdef BUILD_FOR_BLACKBERRY
#   include <bb/pim/calendar/CalendarService>
#   include <bb/pim/calendar/CalendarFolder>
#else
#   include <QOrganizerManager>
#endif

FahrplanCalendarManager::FahrplanCalendarManager(QObject *parent)
    : QAbstractListModel(parent)
    , m_selectedIndex(0)
{
    QHash<int, QByteArray> roles;
    roles.insert(Qt::UserRole, "name");
    setRoleNames(roles);

    settings = new QSettings("smurfy", "fahrplan2", this);

    m_watcher = new QFutureWatcher<void>(this);
    connect(m_watcher, SIGNAL(finished()), SLOT(getCalendarsListFinished()));
    connect(m_watcher, SIGNAL(started()), SIGNAL(selectedCalendarNameChanged()));

    // Change of slectedIndex always changes selectedCalendarName
    connect(this, SIGNAL(selectedIndexChanged()), SIGNAL(selectedCalendarNameChanged()));
}

int FahrplanCalendarManager::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_calendars.count();
}

QVariant FahrplanCalendarManager::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || (index.row() >= m_calendars.count()))
        return QVariant();

    switch (role) {
    case Qt::UserRole:
        return m_calendars.at(index.row()).name;
        break;
    }

    return QVariant();
}

int FahrplanCalendarManager::count() const
{
    return rowCount();
}

int FahrplanCalendarManager::selectedIndex() const
{
    return m_selectedIndex;
}

void FahrplanCalendarManager::setSelectedIndex(int index)
{
    if ((index == m_selectedIndex) || (index >= m_calendars.count()))
        return;

    m_selectedIndex = index;
    emit selectedIndexChanged();

    settings->beginGroup("Calendar");
    if (index > 0) {
#ifdef BUILD_FOR_BLACKBERRY
        settings->setValue("AccountId", m_calendars.at(index).accountId);
        settings->setValue("FolderId", m_calendars.at(index).folderId);
#else
        settings->setValue("CollectionId", m_calendars.at(index).collectionId);
#endif
    } else {
#ifdef BUILD_FOR_BLACKBERRY
        settings->remove("AccountId");
        settings->remove("FolderId");
#else
        settings->remove("CollectionId");
#endif
    }
    settings->endGroup();
}

QString FahrplanCalendarManager::selectedCalendarName() const
{
    if (m_watcher->isRunning())
        return tr("<loading calendars list...>");
    if ((m_selectedIndex < 0) || (m_selectedIndex >= m_calendars.count()))
        return tr("<invalid calendar>");
    else if (m_selectedIndex == 0)
        return tr("Default Calendar");

    return m_calendars.at(m_selectedIndex).name;
}

void FahrplanCalendarManager::reload()
{
    if (m_watcher->isRunning())
        m_watcher->waitForFinished();

    beginResetModel();

    m_calendars.clear();
    m_calendars << CalendarInfo(tr("Default Calendar"));
    m_selectedIndex = 0;

    // Run fetch in a separate thread.
    QFuture<void> future = QtConcurrent::run(this, &FahrplanCalendarManager::getCalendarsList);
    m_watcher->setFuture(future);

    // endResetModel() will be called in getCalendarsListFinished() function.
}

void FahrplanCalendarManager::getCalendarsList()
{
    settings->beginGroup("Calendar");
#ifdef BUILD_FOR_BLACKBERRY
    int accountId = settings->value("AccountId", -1).toInt();
    int folderId = settings->value("FolderId", -1).toInt();

    bb::pim::calendar::CalendarService service;
    QList<bb::pim::calendar::CalendarFolder> folders = service.folders();
    foreach (const bb::pim::calendar::CalendarFolder &folder, folders) {
        if (!folder.isVisible() || folder.isReadOnly())
            continue;

        m_calendars << CalendarInfo(folder.name(), folder.accountId(), folder.id());
        if ((folder.id() == folderId) && (folder.accountId() == accountId)) {
            m_selectedIndex = m_calendars.count() - 1;
            emit selectedIndexChanged();
        }
    }
#else
    QString id = settings->value("CollectionId").toString();
    QOrganizerCollectionId collectionId = QOrganizerCollectionId::fromString(id);

    QOrganizerManager manager;
    QList<QOrganizerCollection> collections = manager.collections();
    foreach (const QOrganizerCollection &collection, collections) {
        // TODO: Find out ??? where possible.
        // Hidden/disabled calendar:
        // - IsVisible on Harmattan;
        // - Enabled on Symbian;
        // - ??? on Ubuntu Touch.
        // Read only calendar:
        // - IsReadOnly on Harmattan;
        // - ??? on Symbian;
        // - ??? on Ubuntu Touch.
        // Calendar doesn't store events (stores only notes, todos, etc.):
        // - EventsAllowed on Harmattan;
        // - ??? on Symbian;
        // - ??? on Ubuntu Touch.
        // ??? temporary calendar ???:
        // - IsRunTimeOnly on Harmattan;
        // - ??? on Symbian;
        // - ??? on Ubuntu Touch.
        if (
#ifdef BUILD_FOR_HARMATTAN
                collection.metaData("IsReadOnly").toBool()
                || !collection.metaData("IsVisible").toBool()
                || !collection.metaData("EventsAllowed").toBool()
                || collection.metaData("IsRunTimeOnly").toBool()
#elif defined(BUILD_FOR_SYMBIAN)
                !collection.metaData("Enabled").toBool()
#else
                // Show all calendars by default on
                // unknown/unsupported platforms.
                false
#endif
                )
            continue;

        m_calendars << CalendarInfo(collection.metaData(QOrganizerCollection::KeyName).toString(), collection.id().toString());
        if (collection.id() == collectionId)
            m_selectedIndex = m_calendars.count() - 1;
    }
#endif
    settings->endGroup();
}

void FahrplanCalendarManager::getCalendarsListFinished()
{
    endResetModel();
    emit countChanged();
    emit selectedIndexChanged();
}
