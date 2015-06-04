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

#ifndef FAHRPLAN_CALENDAR_MANAGER_H
#define FAHRPLAN_CALENDAR_MANAGER_H

#include <QAbstractListModel>

#if !defined(BUILD_FOR_BLACKBERRY) && !defined(BUILD_FOR_SAILFISHOS) && !defined(BUILD_FOR_DESKTOP) && !defined(BUILD_FOR_UBUNTU)
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
#elif defined(BUILD_FOR_SAILFISHOS) && defined(BUILD_FOR_OPENREPOS)
    QString notebookUID;

    CalendarInfo(const QString &name, const QString &notebookUID = QString())
        : name(name), notebookUID(notebookUID) {}
#else
    QString collectionId;

    CalendarInfo(const QString &name, const QString &collectionId = QString())
        : name(name), collectionId(collectionId) {}
#endif
};

class QSettings;
#ifndef QT_NO_CONCURRENT
template <typename T>
class QFutureWatcher;
#endif

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
    QHash<int, QByteArray> roleNames() const;
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
#ifndef QT_NO_CONCURRENT
    QFutureWatcher<void> *m_watcher;
#endif

    QList<CalendarInfo> m_calendars;
    int m_selectedIndex;

    void getCalendarsList();
    QString normalizeCalendarName(QString name);

private slots:
    void getCalendarsListFinished();
};

#endif // FAHRPLAN_CALENDAR_MANAGER_H
