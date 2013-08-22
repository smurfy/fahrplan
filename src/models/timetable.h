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

#ifndef TIMETABLERESULTS_H
#define TIMETABLERESULTS_H

#include "parser/parser_definitions.h"

#include <QAbstractListModel>

class Timetable: public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    enum DisplayRoles {
        CurrentStation = Qt::UserRole,
        DestinationStation,
        TrainType,
        Platform,
        Time,
        MiscInfo,
        Latitude,
        Longitude
    };

    explicit Timetable(QObject *parent = 0);

    QHash<int, QByteArray> roleNames() const;

    int count() const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = CurrentStation) const;

    void setTimetableEntries(const TimetableEntriesList &list);

public slots:
    void clear();

signals:
    void countChanged();

private:
    TimetableEntriesList m_list;
};

#endif // TIMETABLERESULTS_H
