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

#include "fahrplan_parser_thread.h"
#include "models/favorites.h"

#include <QSettings>

Favorites::Favorites(Fahrplan *parent)
    : StationsListModel(parent)
{
    m_settings = new QSettings(FAHRPLAN_SETTINGS_NAMESPACE, "fahrplan2", this);
}

QVariant Favorites::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || (index.row() < 0) || (index.row() >= count()))
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
        StationsList list = stationsList();
        list.append(station);
        qSort(list);
        setStationsList(list);
        saveToSettings();
    }
}

void Favorites::removeFromFavorites(const Station &station)
{
    int index = indexOf(station);
    if (index < 0)
        return;

    removeFromFavorites(index);
}

void Favorites::removeFromFavorites(int index)
{
    if ((index < 0) || (index >= count()))
        return;

    removeAt(index);
    saveToSettings();
}

void Favorites::reload()
{
    if (!m_settings->group().isEmpty())
        m_settings->endGroup();
    m_settings->beginGroup(qobject_cast<Fahrplan *>(QObject::parent())->parser()->uid());

    clear();
    loadFavorites();
}

bool Favorites::isFavorite(const Station &station) const
{
    return contains(station);
}

void Favorites::loadFavorites()
{
    StationsList list;
    int size = m_settings->beginReadArray("favorites");
    for (int k = 0; k < size; ++k) {
        m_settings->setArrayIndex(k);

        Station station;
        station.id = m_settings->value("id");
        station.name = m_settings->value("name").toString();
        list << station;
    }
    m_settings->endArray();
    qSort(list);

    setStationsList(list);
}

void Favorites::saveToSettings()
{
    // Clean up before storing. If we don't do it and the list is shorter than
    // the previous one was, there will be junk entries left in the .ini file.
    m_settings->remove("favorites");

    m_settings->beginWriteArray("favorites");
    int size = count();
    for (int k = 0; k < size; ++k) {
        m_settings->setArrayIndex(k);
        m_settings->setValue("id", at(k).id);
        m_settings->setValue("name", at(k).name);
    }
    m_settings->endArray();
}
