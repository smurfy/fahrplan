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

    QHash<int, QByteArray> roleNames() const;
    int count() const;
    QString get(int i) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

signals:
    void countChanged();
};

#endif // TRAINRESTRICTIONS_H
