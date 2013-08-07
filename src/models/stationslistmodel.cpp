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

#include "stationslistmodel.h"

#include "fahrplan_favorites_manager.h"

StationsListModel::StationsListModel(Fahrplan *fahrplan)
    : QAbstractListModel(fahrplan)
{
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    setRoleNames(roleNames());
#endif
}

QHash<int, QByteArray> StationsListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(Id, "id");
    roles.insert(Name, "name");
    roles.insert(Type, "type");
    roles.insert(IsFavorite, "isFavorite");
    roles.insert(MiscInfo, "miscInfo");
    roles.insert(Latitude, "latitude");
    roles.insert(Longitude, "longitude");
    return roles;
}

int StationsListModel::count() const
{
    return rowCount();
}

int StationsListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_list.count();
}

QVariant StationsListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || (index.row() < 0) || (index.row() >= m_list.count()))
        return QVariant();

    Station item = m_list.at(index.row());
    switch (role) {
    case Id:
        return item.id;
    case Name:
        return item.name;
    case Type:
        return item.type;
    case  IsFavorite:
        return qobject_cast<Fahrplan *>(QObject::parent())->favorites()->isFavorite(item.name);
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

Station StationsListModel::getStation(int index) const
{
    if (index < 0 || index >= m_list.count())
        return Station(false);

    return m_list.at(index);
}

void StationsListModel::setStationsList(const StationsList &list)
{
    beginResetModel();
    m_list = list;
    endResetModel();
    emit countChanged();
}

void StationsListModel::selectStation(int type, int index)
{
    if (index < 0 || index >= m_list.count())
        return;

    emit stationSelected(static_cast<Fahrplan::StationType>(type), m_list.at(index));
}

void StationsListModel::addToFavorites(int index)
{
    if (index < 0 || index >= m_list.count())
        return;

    qobject_cast<Fahrplan *>(QObject::parent())->favorites()->addFavorite(m_list.at(index).name);
    QModelIndex i = this->index(index, 0);
    emit dataChanged(i, i);
}

void StationsListModel::removeFromFavorites(int index)
{
    if (index < 0 || index >= m_list.count())
        return;

    qobject_cast<Fahrplan *>(QObject::parent())->favorites()->removeFavorite(m_list.at(index).name);
    QModelIndex i = this->index(index, 0);
    emit dataChanged(i, i);
}
