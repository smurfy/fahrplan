/****************************************************************************
**
**  This file is a part of Fahrplan.
**
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation; either version 2 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License along
**  with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include "fahrplan_calendar_manager.h"

#include <QSettings>
#ifndef QT_NO_CONCURRENT
#   include <QtConcurrentRun>
#   include <QFutureWatcher>
#endif

#ifdef BUILD_FOR_BLACKBERRY
#   include <bb/pim/calendar/CalendarService>
#   include <bb/pim/calendar/CalendarFolder>
#   include <bb/pim/account/AccountService>
#   include <bb/pim/account/Account>
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

#ifndef QT_NO_CONCURRENT
    m_watcher = new QFutureWatcher<void>(this);
    connect(m_watcher, SIGNAL(finished()), SLOT(getCalendarsListFinished()));
    connect(m_watcher, SIGNAL(started()), SIGNAL(selectedCalendarNameChanged()));
#endif

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
#ifndef QT_NO_CONCURRENT
    if (m_watcher->isRunning())
        return tr("<loading calendars list...>");
#endif
    if ((m_selectedIndex < 0) || (m_selectedIndex >= m_calendars.count()))
        return tr("<invalid calendar>");
    else if (m_selectedIndex == 0)
        return tr("Default Calendar");

    return m_calendars.at(m_selectedIndex).name;
}

void FahrplanCalendarManager::reload()
{
#ifndef QT_NO_CONCURRENT
    if (m_watcher->isRunning())
        m_watcher->waitForFinished();
#endif

    beginResetModel();

    m_calendars.clear();
    m_calendars << CalendarInfo(tr("Default Calendar"));
    m_selectedIndex = 0;

#ifndef QT_NO_CONCURRENT
    // Run fetch in a separate thread.
    QFuture<void> future = QtConcurrent::run(this, &FahrplanCalendarManager::getCalendarsList);
    m_watcher->setFuture(future);
#else
    getCalendarsList();
    getCalendarsListFinished();
#endif

    // endResetModel() is called in getCalendarsListFinished() function.
}

void FahrplanCalendarManager::getCalendarsList()
{
    settings->beginGroup("Calendar");
#ifdef BUILD_FOR_BLACKBERRY
    int accountId = settings->value("AccountId", -1).toInt();
    int folderId = settings->value("FolderId", -1).toInt();

    bb::pim::calendar::CalendarService service;
    bb::pim::account::AccountService accservice;
    QList<bb::pim::calendar::CalendarFolder> folders = service.folders();
    foreach (const bb::pim::calendar::CalendarFolder &folder, folders) {
        if (!folder.isVisible() || folder.isReadOnly())
            continue;

        QString account;
        if (folder.accountId() == 1)
            account = tr("Local Calendar");
        else
            account = accservice.account(folder.accountId()).displayName();
        m_calendars << CalendarInfo(tr("%1 (%2)", "Calendar name (Account name)")
                                    .arg(folder.name()).arg(account)
                                    , folder.accountId(), folder.id());
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
