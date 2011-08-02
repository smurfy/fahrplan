/*******************************************************************************

    This file is a part of Fahrplan for maemo 2009-2010

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
#ifndef PARSER_DEFINITIONS_H
#define PARSER_DEFINITIONS_H

#include <QObject>
#include <QDebug>
#include <qdeclarative.h>
#include <QDeclarativeListProperty>

namespace Fahrplan
{
    enum curReqStates {
        noneRequest,
        canceledRequest,
        stationsByNameRequest,
        stationsByCoordinatesRequest
    };
}

class StationsResultItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString stationName READ stationName WRITE setStationName)
    Q_PROPERTY(QString stationTyp READ stationType WRITE setStationType)
    Q_PROPERTY(QString miscInfo READ miscInfo WRITE setMiscInfo)
    Q_PROPERTY(qreal longitude READ longitude WRITE setLongitude)
    Q_PROPERTY(qreal latitude READ latitude WRITE setLatitude)

    public:
        QString stationName() const;
        void setStationName(const QString &);
        QString stationType() const;
        void setStationType(const QString &);
        QString miscInfo() const;
        void setMiscInfo(const QString &);
        qreal longitude();
        void setLongitude(qreal);
        qreal latitude();
        void setLatitude(qreal);
    private:
        QString m_stationName;
        QString m_stationType;
        QString m_miscInfo;
        qreal m_longitude;
        qreal m_latitude;
};


class StationsResultList : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal count READ itemcount)
    public slots:
        StationsResultItem *getItem(int);
    public:
        void appendItem(StationsResultItem *item);
        qreal itemcount();
    private:
        QList<StationsResultItem*> m_items;
};


#endif // PARSER_DEFINITIONS_H
