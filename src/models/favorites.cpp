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

#include "models/favorites.h"

#include <QSettings>

Favorites::Favorites(QObject *parent)
    : StationsListModel(parent)
{
    m_settings = new QSettings("smurfy", "fahrplan2", this);

    int size = m_settings->beginReadArray("favorites");
    for (int k = 0; k < size; ++k) {
        m_settings->setArrayIndex(k);

        Station station;
        station.id = m_settings->value("id");
        station.name = m_settings->value("name").toString();
        m_list << station;
    }
    m_settings->endArray();
    qSort(m_list);
}

QVariant Favorites::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || (index.row() < 0) || (index.row() >= m_list.count()))
        return QVariant();

    if (role == IsFavorite)
        return true;
    else if (role == MiscInfo)
        return "";
    else
        return StationsListModel::data(index, role);
}

void Favorites::addToFavorites(const Station &station)
{
    if (!isFavorite(station)) {
        beginResetModel();
        m_list.append(station);
        qSort(m_list);
        endResetModel();

        emit countChanged();

        saveToSettings();
    }
}

void Favorites::removeFromFavorites(const Station &station)
{
    int index = m_list.indexOf(station);
    if (index < 0)
        return;

    removeFromFavorites(index);
}

void Favorites::removeFromFavorites(int index)
{
    if ((index < 0) || (index >= m_list.count()))
        return;

    beginRemoveRows(QModelIndex(), index, index);
    m_list.removeAt(index);
    endRemoveRows();

    emit countChanged();

    saveToSettings();
}

bool Favorites::isFavorite(const Station &station) const
{
    return m_list.contains(station);
}

void Favorites::saveToSettings()
{
    m_settings->beginWriteArray("favorites");
    int size = m_list.size();
    for (int k = 0; k < size; ++k) {
        m_settings->setArrayIndex(k);
        m_settings->setValue("id", m_list.at(k).id);
        m_settings->setValue("name", m_list.at(k).name);
    }
    m_settings->endArray();
}
