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

#include <QSettings>
#include <QStringList>

class FahrplanFavoritesManager : public QObject
{
    Q_OBJECT

    public:
        explicit FahrplanFavoritesManager(QObject *parent = 0);

    public slots:
        void addFavorite(const QString &name);
        void removeFavorite(const QString &name);
        bool isFavorite(const QString &name);
        QStringList getFavorites();

    signals:
        void favoritesChanged(QStringList);

    private:
        QStringList favorites;
        QSettings *settings;
};

#endif // FAHRPLAN_FAVORITES_MANAGER_H
