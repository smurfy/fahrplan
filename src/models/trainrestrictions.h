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

#ifndef TRAINRESTRICTIONS_H
#define TRAINRESTRICTIONS_H

#include <QStringListModel>

class Trainrestrictions : public QStringListModel
{
    Q_OBJECT
public:
    Q_PROPERTY(int count READ count NOTIFY countChanged)

    explicit Trainrestrictions(QObject *parent = 0);
    int count() const;

    QHash<int, QByteArray> roleNames() const;

    void setStringList(const QStringList &strings);
    QVariant data(const QModelIndex &index, int role) const;
    Q_INVOKABLE int rowCount(const QModelIndex &parent) const;
    Q_INVOKABLE int columnCount(const QModelIndex & parent = QModelIndex()) const;
    Q_INVOKABLE QVariant get(int i) const;
    Q_INVOKABLE QVariant data(int index) const;
    
signals:
    void countChanged();
};

#endif // TRAINRESTRICTIONS_H
