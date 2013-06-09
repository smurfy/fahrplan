#ifndef FAHRPLAN_CALENDAR_MANAGER_H
#define FAHRPLAN_CALENDAR_MANAGER_H

#include <QAbstractListModel>

#ifndef BUILD_FOR_BLACKBERRY
#   include <QOrganizerCollectionId>
QTM_USE_NAMESPACE
#endif

struct CalendarInfo
{
    QString name;
#ifdef BUILD_FOR_BLACKBERRY
    int accountId;
    int folderId;

    CalendarInfo(const QString &name, int accountId = -1, int folderId = -1)
        : name(name), accountId(accountId), folderId(folderId) {}
#else
    QString collectionId;

    CalendarInfo(const QString &name, const QString &collectionId = QString())
        : name(name), collectionId(collectionId) {}
#endif
};

class QSettings;
template <typename T>
class QFutureWatcher;

class FahrplanCalendarManager: public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(int selectedIndex READ selectedIndex WRITE setSelectedIndex NOTIFY selectedIndexChanged)
    Q_PROPERTY(QString selectedCalendarName READ selectedCalendarName NOTIFY selectedCalendarNameChanged)

public:
    explicit FahrplanCalendarManager(QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    int count() const;

    int selectedIndex() const;
    void setSelectedIndex(int index);

    QString selectedCalendarName() const;

    Q_INVOKABLE void reload();

signals:
    void countChanged();
    void selectedIndexChanged();
    void selectedCalendarNameChanged();

private:
    QSettings *settings;
    QFutureWatcher<void> *m_watcher;

    QList<CalendarInfo> m_calendars;
    int m_selectedIndex;

    void getCalendarsList();

private slots:
    void getCalendarsListFinished();
};

#endif // FAHRPLAN_CALENDAR_MANAGER_H
