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

#ifndef STATIONSLISTMODEL_H
#define STATIONSLISTMODEL_H

#include "fahrplan.h"
#include "parser/parser_definitions.h"

#include <QAbstractListModel>

#ifdef Q_OS_BLACKBERRY
namespace bb { namespace cascades { class DataModel; } }
#endif

class StationsListModel: public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    enum DisplayRoles {
        Id = Qt::UserRole,
        Name,
        Type,
        IsFavorite,
        MiscInfo,
        Latitude,
        Longitude
    };

    explicit StationsListModel(QObject *parent = 0);

    QHash<int, QByteArray> roleNames() const;

    int count() const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Name) const;

    void setStationsList(const StationsList &list);

public slots:
    void clear();
    void selectStation(int type, int index);

signals:
    void countChanged();
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void stationSelected(Fahrplan::StationType type, const Station &station);

protected:
    StationsList stationsList() const;

    bool contains(const Station &station) const;
    int indexOf(const Station &station) const;
    Station at(int index) const;
    void removeAt(int index);

private:
    StationsList m_list;

#ifdef Q_OS_BLACKBERRY
    Q_PROPERTY(bb::cascades::DataModel *dataModel READ dataModel CONSTANT)
public:
    bb::cascades::DataModel *dataModel() const;
private:
    bb::cascades::DataModel *m_dataModel;
#endif
};

#endif // STATIONSLISTMODEL_H
