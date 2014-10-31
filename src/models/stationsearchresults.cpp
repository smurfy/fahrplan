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

#include "stationsearchresults.h"

#include "fahrplan.h"
#include "favorites.h"

StationSearchResults::StationSearchResults(Fahrplan *parent)
    : StationsListModel(parent)
{
    // When favorites change, we need to trigger view update
    // so that stars states in the search results are updated.
    connect(parent->favorites(), SIGNAL(countChanged()), this, SLOT(onCountChanged()));
}

QVariant StationSearchResults::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || (index.row() < 0) || (index.row() >= count()))
        return QVariant();

    Station item = at(index.row());
    if (role == IsFavorite)
        return qobject_cast<Fahrplan *>(QObject::parent())->favorites()->isFavorite(item);
    else
        return StationsListModel::data(index, role);
}

void StationSearchResults::addToFavorites(int index)
{
    if (index < 0 || index >= count())
        return;

    qobject_cast<Fahrplan *>(QObject::parent())->favorites()->addToFavorites(at(index));
    QModelIndex i = this->index(index, 0);
    emit dataChanged(i, i);
}

void StationSearchResults::removeFromFavorites(int index)
{
    if (index < 0 || index >= count())
        return;

    qobject_cast<Fahrplan *>(QObject::parent())->favorites()->removeFromFavorites(at(index));
    QModelIndex i = this->index(index, 0);
    emit dataChanged(i, i);
}

void StationSearchResults::onCountChanged()
{
    emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, 0));
}
