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

#include "journeyresults.h"

JourneyResults::JourneyResults(QObject *parent) :
    QAbstractListModel(parent)
{
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    setRoleNames(roleNames());
#endif
}

QHash<int, QByteArray> JourneyResults::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(Id, "id");
    roles.insert(Date, "date");
    roles.insert(DepartureTime, "departureTime");
    roles.insert(ArrivalTime, "arrivalTime");
    roles.insert(TrainType, "trainType");
    roles.insert(Duration, "duration");
    roles.insert(Transfers, "transfers");
    roles.insert(MiscInfo, "miscInfo");
    return roles;
}

int JourneyResults::count() const
{
    return rowCount();
}

int JourneyResults::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_list.items.count();
}

QVariant JourneyResults::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || (index.row() < 0) || (index.row() >= m_list.items.count()))
        return QVariant();

    JourneyResultItem item = m_list.items.at(index.row());

    switch (role) {
    case Id:
        return item.id;
    case Date:
        return item.departureDateTime.date();
    case DepartureTime:
        return item.departureDateTime;
    case ArrivalTime:
        return item.arrivalDateTime;
    case TrainType:
        return item.trainType;
    case Duration:
        return item.duration;
    case Transfers:
        return item.transfers;
    case MiscInfo:
        return item.miscInfo;
    default:
        return QVariant();
    }
}

const Station &JourneyResults::departureStation() const
{
    return m_list.departureStation;
}

const Station &JourneyResults::arrivalStation() const
{
    return m_list.arrivalStation;
}

const Station &JourneyResults::viaStation() const
{
    return m_list.viaStation;
}

const QString JourneyResults::timeInfo() const
{
    return m_list.timeInfo;
}

void JourneyResults::setJourneyResults(JourneyResultHeader list)
{
    beginResetModel();
    m_list = list;
    endResetModel();
    emit countChanged();
}

void JourneyResults::clear()
{
    beginResetModel();
    //
    endResetModel();
    emit countChanged();
}
