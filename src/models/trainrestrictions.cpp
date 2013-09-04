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

#include "trainrestrictions.h"

Trainrestrictions::Trainrestrictions(QObject *parent)
    : QStringListModel(parent)
{
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    setRoleNames(roleNames());
#endif
}

QHash<int, QByteArray> Trainrestrictions::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(Qt::DisplayRole, "name");
    return roles;
}

QVariant Trainrestrictions::get(int i) const
{
    if (i >= 0 && i < count()) {
        return QVariant(stringList().at(i));
    }
    return QVariant(tr("None"));
}

QVariant Trainrestrictions::data(const QModelIndex &index, int role) const
{
    if (index.isValid()) {
        return QStringListModel::data(index, role);
    }
    return QVariant(tr("None"));
}

QVariant Trainrestrictions::data(int index) const
{
    if (index >= 0 && index < count()) {
        return stringList().at(index);
    }
    return QVariant(tr("None"));
}

int Trainrestrictions::columnCount(const QModelIndex & parent) const
{
    return 1;
}

int Trainrestrictions::rowCount(const QModelIndex &parent) const
{
    return stringList().count();
}

int Trainrestrictions::count() const
{
    return stringList().count();
}

void Trainrestrictions::setStringList(const QStringList &strings)
{
    QStringListModel::setStringList(strings);
    emit countChanged();
}
