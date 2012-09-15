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

#ifndef PARSER_DEFINITIONS_H
#define PARSER_DEFINITIONS_H

#include <QObject>
#include <QDebug>
#include <QDate>
#include <qdeclarative.h>
#include <QDeclarativeListProperty>

namespace FahrplanNS
{
    enum curReqStates {
        noneRequest,
        stationsByNameRequest,
        stationsByCoordinatesRequest,
        searchJourneyRequest,
        searchJourneyLaterRequest,
        searchJourneyEarlierRequest,
        journeyDetailsRequest,
        getTimeTableForStationRequest
    };
}

class TimeTableResultItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString stationName READ stationName WRITE setStationName)
    Q_PROPERTY(QString trainType READ trainType WRITE setTrainType)
    Q_PROPERTY(QString destinationName READ destinationName WRITE setDestinationName)
    Q_PROPERTY(QTime time READ time WRITE setTime)
    Q_PROPERTY(QString platform READ platform WRITE setPlatform)
    Q_PROPERTY(qreal longitude READ longitude WRITE setLongitude)
    Q_PROPERTY(qreal latitude READ latitude WRITE setLatitude)
    Q_PROPERTY(QString miscInfo READ miscInfo WRITE setMiscInfo)

    public:
        QString stationName() const;
        void setStationName(const QString &);
        QString trainType() const;
        void setTrainType(const QString &);
        QString destinationName() const;
        void setDestinationName(const QString &);
        QTime time() const;
        void setTime(const QTime &);
        QString platform() const;
        void setPlatform(const QString &);
        qreal longitude();
        void setLongitude(qreal);
        qreal latitude();
        void setLatitude(qreal);
        QString miscInfo() const;
        void setMiscInfo(const QString &);
    private:
        QString m_stationName;
        QString m_trainType;
        QString m_destinationName;
        QTime m_time;
        QString m_platform;
        qreal m_longitude;
        qreal m_latitude;
        QString m_miscInfo;
};

class TimeTableResultList : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal count READ itemcount)
    public slots:
        TimeTableResultItem *getItem(int);
    public:
        void appendItem(TimeTableResultItem *item);
        qreal itemcount();
    private:
        QList<TimeTableResultItem*> m_items;
};

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

class JourneyResultItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id WRITE setId)
    //Q_PROPERTY(QDate date READ date WRITE setDate)
    Q_PROPERTY(QString departureTime READ departureTime WRITE setDepartureTime)
    Q_PROPERTY(QString arrivalTime READ arrivalTime WRITE setArrivalTime)
    Q_PROPERTY(QString trainType READ trainType WRITE setTrainType)
    Q_PROPERTY(QString duration READ duration WRITE setDuration)
    Q_PROPERTY(QString transfers READ transfers WRITE setTransfers)
    Q_PROPERTY(QString miscInfo READ miscInfo WRITE setMiscInfo)

    //Some Internal Data fields, primarly to store additional data per backend, like the details url
    Q_PROPERTY(QString internalData1 READ internalData1 WRITE setInternalData1)
    Q_PROPERTY(QString internalData2 READ internalData2 WRITE setInternalData2)

    public:
        QString id() const;
        void setId(const QString &);
        QDate date() const;
        void setDate(const QDate &);
        QString departureTime() const;
        void setDepartureTime(const QString &);
        QString arrivalTime() const;
        void setArrivalTime(const QString &);
        QString trainType() const;
        void setTrainType(const QString &);
        QString duration() const;
        void setDuration(const QString &);
        QString transfers() const;
        void setTransfers(const QString &);
        QString miscInfo() const;
        void setMiscInfo(const QString &);
        QString internalData1() const;
        void setInternalData1(const QString &);
        QString internalData2() const;
        void setInternalData2(const QString &);
    private:
        QString m_id;
        QDate m_date;
        QString m_departureTime;
        QString m_arrivalTime;
        QString m_trainType;
        QString m_duration;
        QString m_transfers;
        QString m_miscInfo;
        QString m_internalData1;
        QString m_internalData2;
};

class JourneyResultList : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal count READ itemcount)
    Q_PROPERTY(QString departureStation READ departureStation WRITE setDepartureStation)
    Q_PROPERTY(QString viaStation READ viaStation WRITE setViaStation)
    Q_PROPERTY(QString arrivalStation READ arrivalStation WRITE setArrivalStation)
    Q_PROPERTY(QString timeInfo READ timeInfo WRITE setTimeInfo)

    public slots:
        JourneyResultItem *getItem(int);
    public:
        void appendItem(JourneyResultItem *item);
        qreal itemcount();
        QString departureStation() const;
        void setDepartureStation(const QString &);
        QString arrivalStation() const;
        void setArrivalStation(const QString &);
        QString viaStation() const;
        void setViaStation(const QString &);
        QString timeInfo() const;
        void setTimeInfo(const QString &);
    private:
        QList<JourneyResultItem*> m_items;
        QString m_departureStation;
        QString m_viaStation;
        QString m_arrivalStation;
        QString m_timeInfo;
};


class JourneyDetailResultItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString departureStation READ departureStation WRITE setDepartureStation)
    Q_PROPERTY(QString departureInfo READ departureInfo WRITE setDepartureInfo)
    Q_PROPERTY(QDateTime departureDateTime READ departureDateTime WRITE setDepartureDateTime)
    Q_PROPERTY(QString arrivalStation READ arrivalStation WRITE setArrivalStation)
    Q_PROPERTY(QString arrivalInfo READ arrivalInfo WRITE setArrivalInfo)
    Q_PROPERTY(QDateTime arrivalDateTime READ arrivalDateTime WRITE setArrivalDateTime)
    Q_PROPERTY(QString info READ info WRITE setInfo)
    Q_PROPERTY(QString train READ train WRITE setTrain)

    //Some Internal Data fields, primarly to store additional data per backend, like the details url
    Q_PROPERTY(QString internalData1 READ internalData1 WRITE setInternalData1)
    Q_PROPERTY(QString internalData2 READ internalData2 WRITE setInternalData2)
    public:
        QString departureStation() const;
        void setDepartureStation(const QString &);
        QString departureInfo() const;
        void setDepartureInfo(const QString &);
        QDateTime departureDateTime() const;
        void setDepartureDateTime(const QDateTime &);
        QString arrivalStation() const;
        void setArrivalStation(const QString &);
        QString arrivalInfo() const;
        void setArrivalInfo(const QString &);
        QDateTime arrivalDateTime() const;
        void setArrivalDateTime(const QDateTime &);
        QString info() const;
        void setInfo(const QString &);
        QString train() const;
        void setTrain(const QString &);
        QString internalData1() const;
        void setInternalData1(const QString &);
        QString internalData2() const;
        void setInternalData2(const QString &);
    private:
        QString m_departureStation;
        QString m_departureInfo;
        QDateTime m_departureDateTime;
        QString m_arrivalStation;
        QString m_arrivalInfo;
        QDateTime m_arrivalDateTime;
        QString m_info;
        QString m_train;
        QString m_internalData1;
        QString m_internalData2;
};

class JourneyDetailResultList : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id WRITE setId)
    Q_PROPERTY(qreal count READ itemcount)
    Q_PROPERTY(QString duration READ duration WRITE setDuration)
    Q_PROPERTY(QString info READ info WRITE setInfo)
    Q_PROPERTY(QString departureStation READ departureStation WRITE setDepartureStation)
    Q_PROPERTY(QDateTime departureDateTime READ departureDateTime WRITE setDepartureDateTime)
    Q_PROPERTY(QString viaStation READ viaStation WRITE setViaStation)
    Q_PROPERTY(QString arrivalStation READ arrivalStation WRITE setArrivalStation)
    Q_PROPERTY(QDateTime arrivalDateTime READ arrivalDateTime WRITE setArrivalDateTime)

    public slots:
        JourneyDetailResultItem *getItem(int);
    public:
        void appendItem(JourneyDetailResultItem *item);
        qreal itemcount();
        QString id() const;
        void setId(const QString &);
        QString departureStation() const;
        void setDepartureStation(const QString &);
        QDateTime departureDateTime() const;
        void setDepartureDateTime(const QDateTime &);
        QString viaStation() const;
        void setViaStation(const QString &);
        QString arrivalStation() const;
        void setArrivalStation(const QString &);
        QDateTime arrivalDateTime() const;
        void setArrivalDateTime(const QDateTime &);
        QString info() const;
        void setInfo(const QString &);
        QString duration() const;
        void setDuration(const QString &);
    private:
        QList<JourneyDetailResultItem*> m_items;
        QString m_id;
        QString m_departureStation;
        QDateTime m_departureDateTime;
        QString m_viaStation;
        QString m_arrivalStation;
        QDateTime m_arrivalDateTime;
        QString m_info;
        QString m_duration;
};

#endif // PARSER_DEFINITIONS_H
