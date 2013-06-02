/*******************************************************************************

    This file is a part of Fahrplan for maemo 2009-2012

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

#ifndef FAHRPLAN_FAVORITES_MANAGER_H
#define FAHRPLAN_FAVORITES_MANAGER_H

#include <QSettings>
#include <QStringList>
#include <QAbstractListModel>

class FahrplanFavoritesManager: public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)

    public:
        enum DisplayRoles {
            Name = Qt::UserRole,
            Process,
            Internal,
            IsFavorite,
            ShowFavorite,
            MiscInfo
        };

        explicit FahrplanFavoritesManager(QObject *parent = 0);

        int count() const;

        QHash<int, QByteArray> roleNames() const;
        int rowCount(const QModelIndex &parent = QModelIndex()) const;
        QVariant data(const QModelIndex &index, int role = Name) const;

    public slots:
        void addFavorite(const QString &index);
        void removeFavorite(const QString &index);
        void removeFavorite(int index);
        bool isFavorite(const QString &name) const;
        QStringList getFavorites();

    signals:
        void countChanged();
        void favoritesChanged(QStringList);

    private:
        QStringList favorites;
        QSettings *settings;
};

#endif // FAHRPLAN_FAVORITES_MANAGER_H
