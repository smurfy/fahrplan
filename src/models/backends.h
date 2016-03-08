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

#ifndef BACKENDS_H
#define BACKENDS_H

#include <QAbstractListModel>
#include <QStringList>

class Backends :public QAbstractListModel
{
    Q_OBJECT

public:
    Q_PROPERTY(int count READ count NOTIFY countChanged)

    enum DisplayRoles {
        Name = Qt::UserRole,
        Id,
    };

    explicit Backends(QObject *parent = 0);

    QHash<int, QByteArray> roleNames() const;

    int count() const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Name) const;
    void setBackendParserList(const QStringList &list);

signals:
    void countChanged();

public slots:
    int getItemIndexForParserId(int parserId) const;
    int getParserIdForItemIndex(int index) const;

private:
    QStringList m_list;
    QList<int> m_ordered;
};

#endif // BACKENDS_H
