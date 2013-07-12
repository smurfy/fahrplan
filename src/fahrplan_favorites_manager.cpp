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

#include "fahrplan_favorites_manager.h"

FahrplanFavoritesManager::FahrplanFavoritesManager(QObject *parent) :
    QObject(parent)
{
    settings = new QSettings("smurfy", "fahrplan2");
    favorites = settings->value("favorites", favorites).toStringList();
    favorites.sort();
}

QStringList FahrplanFavoritesManager::getFavorites()
{
    return favorites;
}

void FahrplanFavoritesManager::addFavorite(const QString &name)
{
    if (!isFavorite(name)) {
        favorites.append(name);
        favorites.removeDuplicates();
        favorites.sort();
        emit favoritesChanged(favorites);
        settings->setValue("favorites", favorites);
    }
}

void FahrplanFavoritesManager::removeFavorite(const QString &name)
{
    if (isFavorite(name)) {
        int index = favorites.indexOf(name);
        favorites.removeAt(index);
        favorites.sort();
        emit favoritesChanged(favorites);
        settings->setValue("favorites", favorites);
    }
}

bool FahrplanFavoritesManager::isFavorite(const QString &name)
{
    return favorites.contains(name);
}
