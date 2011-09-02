/*******************************************************************************

    This file is a part of Fahrplan for maemo 2009-2011

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

*/

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

void FahrplanFavoritesManager::addFavorite(QString name)
{
    if (!isFavorite(name)) {
        favorites.append(name);
        favorites.removeDuplicates();
        favorites.sort();
        emit favoritesChanged(favorites);
        settings->setValue("favorites", favorites);
    }
}

void FahrplanFavoritesManager::removeFavorite(QString name)
{
    if (isFavorite(name)) {
        int index = favorites.indexOf(name);
        favorites.removeAt(index);
        favorites.sort();
        emit favoritesChanged(favorites);
        settings->setValue("favorites", favorites);
    }
}

bool FahrplanFavoritesManager::isFavorite(QString name)
{
    return favorites.contains(name);
}
