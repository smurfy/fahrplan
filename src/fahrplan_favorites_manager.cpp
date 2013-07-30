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
    QAbstractListModel(parent)
{
    settings = new QSettings("smurfy", "fahrplan2", this);

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    setRoleNames(roleNames());
#endif

    favorites = settings->value("favorites", favorites).toStringList();
    favorites.sort();
}

QHash<int, QByteArray> FahrplanFavoritesManager::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(Name, "name");
    roles.insert(Process, "process");
    roles.insert(Internal, "internal");
    roles.insert(IsFavorite, "isfavorite");
    roles.insert(ShowFavorite, "showfavorite");
    roles.insert(MiscInfo, "miscinfo");
    return roles;
}

int FahrplanFavoritesManager::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return favorites.count();
}

QVariant FahrplanFavoritesManager::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || (index.row() < 0) || (index.row() >= favorites.count()))
        return QVariant();

    switch (role) {
    case Name:
        return favorites.at(index.row());
        break;
    case Process:
        return false;
        break;
    case Internal:
        return false;
        break;
    case IsFavorite:
        return true;
        break;
    case ShowFavorite:
        return true;
        break;
    case MiscInfo:
        return "";
        break;
    default:
        return QVariant();
    }
}

int FahrplanFavoritesManager::count() const
{
    return rowCount();
}

QStringList FahrplanFavoritesManager::getFavorites()
{
    return favorites;
}

void FahrplanFavoritesManager::addFavorite(const QString &name)
{
    if (!isFavorite(name)) {
        beginResetModel();
        favorites.append(name);
        favorites.removeDuplicates();
        favorites.sort();
        endResetModel();
        emit countChanged();
        emit favoritesChanged(favorites);
        settings->setValue("favorites", favorites);
    }
}

void FahrplanFavoritesManager::removeFavorite(const QString &name)
{
    int index = favorites.indexOf(name);
    if (index >=0)
        removeFavorite(index);
}

void FahrplanFavoritesManager::removeFavorite(int index)
{
    if ((index < 0) || (index >= favorites.count()))
        return;

    beginRemoveRows(QModelIndex(), index, index);
    favorites.removeAt(index);
    endRemoveRows();
    emit countChanged();
    emit favoritesChanged(favorites);
    settings->setValue("favorites", favorites);
}

bool FahrplanFavoritesManager::isFavorite(const QString &name) const
{
    return favorites.contains(name);
}
