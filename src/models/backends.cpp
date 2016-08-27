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

#include "backends.h"

Backends::Backends(QObject *parent)
    : QAbstractListModel(parent)
{
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    setRoleNames(roleNames());
#endif

    connect(this, SIGNAL(modelReset()), this, SIGNAL(countChanged()));
}

QHash<int, QByteArray> Backends::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(Qt::DisplayRole, "modelData");
    roles.insert(Name, "name");
    roles.insert(Id, "id");
    return roles;
}

QVariant Backends::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || (index.row() < 0) || (index.row() >= m_list.count()))
        return QVariant();

    int idx = m_ordered.at(index.row());
    QString item = m_list.at(idx);

    switch (role) {
    case Qt::DisplayRole:
    case Name:
        return item;
    case Id:
        return idx;
    default:
        return QVariant();
    }

}

int Backends::count() const
{
    return rowCount();
}

int Backends::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_list.count();
}

int Backends::getItemIndexForParserId(int parserId) const {
    return m_ordered.indexOf(parserId);
}

int Backends::getParserIdForItemIndex(int index) const {
    if (index > (m_ordered.count() -1) || index < 0) {
        index = getItemIndexForParserId(0);
    }
    return m_ordered.at(index);
}

void Backends::setBackendParserList(const QStringList &list)
{
    beginResetModel();
    m_list = list;
    m_ordered.clear();
    QMap<QString, int> sortList;
    for (int idx = 0; idx < list.count(); idx++) {
        QString str = list.at(idx);
        sortList.insert(str.toLower(), idx);
    }

    m_ordered = sortList.values();

    endResetModel();
    emit countChanged();
}
