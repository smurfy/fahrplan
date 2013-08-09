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

#ifndef FAHRPLAN_FAVORITES_MANAGER_H
#define FAHRPLAN_FAVORITES_MANAGER_H

#include "parser/parser_definitions.h"
#include "models/stationslistmodel.h"

class QSettings;
class Favorites: public StationsListModel
{
    Q_OBJECT

public:
    explicit Favorites(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role = Name) const;

    bool isFavorite(const Station &name) const;

public slots:
    void addToFavorites(const Station &station);
    void removeFromFavorites(const Station &station);
    void removeFromFavorites(int index);

signals:
    void countChanged();

private:
    QSettings *m_settings;

    void saveToSettings();
};

#endif // FAHRPLAN_FAVORITES_MANAGER_H
