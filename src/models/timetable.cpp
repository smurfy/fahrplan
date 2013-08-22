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

#include "timetable.h"

Timetable::Timetable(QObject *parent)
    : QAbstractListModel(parent)
{
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    setRoleNames(roleNames());
#endif
}

QHash<int, QByteArray> Timetable::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(CurrentStation, "currentStation");
    roles.insert(DestinationStation, "destinationStation");
    roles.insert(TrainType, "trainType");
    roles.insert(Platform, "platform");
    roles.insert(Time, "time");
    roles.insert(MiscInfo, "miscInfo");
    roles.insert(Latitude, "latitude");
    roles.insert(Longitude, "longitude");
    return roles;
}

int Timetable::count() const
{
    return rowCount();
}

int Timetable::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_list.count();
}

QVariant Timetable::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || (index.row() < 0) || (index.row() >= m_list.count()))
        return QVariant();

    TimetableEntry item = m_list.at(index.row());
    switch (role) {
    case CurrentStation:
        return item.currentStation;
    case DestinationStation:
        return item.destinationStation;
    case TrainType:
        return item.trainType;
    case Platform:
        return item.platform;
    case Time:
        return item.time;
    case MiscInfo:
        return item.miscInfo;
    case Latitude:
        return item.latitude;
    case Longitude:
        return item.longitude;
    default:
        return QVariant();
    }
}

void Timetable::setTimetableEntries(const TimetableEntriesList &list)
{
    beginResetModel();
    m_list = list;
    endResetModel();
    emit countChanged();
}

void Timetable::clear()
{
    beginResetModel();
    m_list.clear();
    endResetModel();
    emit countChanged();
}
