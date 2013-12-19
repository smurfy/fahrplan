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

#ifndef STATIONSEARCHRESULTS_H
#define STATIONSEARCHRESULTS_H

#include "stationslistmodel.h"

class StationSearchResults: public StationsListModel
{
    Q_OBJECT

public:
    explicit StationSearchResults(Fahrplan *parent = 0);

    QVariant data(const QModelIndex &index, int role) const;

public slots:
    void addToFavorites(int index);
    void removeFromFavorites(int index);

private slots:
    void onCountChanged();
};

#endif // STATIONSEARCHRESULTS_H
