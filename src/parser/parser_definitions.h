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

#ifndef PARSER_DEFINITIONS_H
#define PARSER_DEFINITIONS_H

#include <QObject>
#include <QDate>
#include <QVariant>
#include <QDebug>

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

struct Station
{
    bool valid;
    QVariant id;
    QString name;
    QString type;
    QString miscInfo;
    qreal latitude;
    qreal longitude;

public:
    Station();
    explicit Station(bool isValid);

    bool operator ==(const Station &other) const;
    bool operator <(const Station &other) const;
};


struct StopStation : Station {
    QDateTime departureDateTime;
    QDateTime arrivalDateTime;

public:
    StopStation& operator=(const Station& station);
};


typedef QList<Station> StationsList;
Q_DECLARE_METATYPE(Station)
Q_DECLARE_METATYPE(StopStation)
Q_DECLARE_METATYPE(StationsList)

struct TimetableEntry
{
    StopStation currentStation;
    StopStation destinationStation;
    QString trainType;
    QString platform;
    QString miscInfo;

public:
    TimetableEntry();
};
typedef QList<TimetableEntry> TimetableEntriesList;
Q_DECLARE_METATYPE(TimetableEntry)
Q_DECLARE_METATYPE(TimetableEntriesList)

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
    Q_PROPERTY(Station departureStation READ departureStation WRITE setDepartureStation)
    Q_PROPERTY(Station viaStation READ viaStation WRITE setViaStation)
    Q_PROPERTY(Station arrivalStation READ arrivalStation WRITE setArrivalStation)
    Q_PROPERTY(QString timeInfo READ timeInfo WRITE setTimeInfo)

    public slots:
        JourneyResultItem *getItem(int);
    public:
        void appendItem(JourneyResultItem *item);
        qreal itemcount();
        Station departureStation() const;
        void setDepartureStation(const Station &);
        Station arrivalStation() const;
        void setArrivalStation(const Station &);
        Station viaStation() const;
        void setViaStation(const Station &);
        QString timeInfo() const;
        void setTimeInfo(const QString &);
    private:
        QList<JourneyResultItem*> m_items;
        Station m_departureStation;
        Station m_viaStation;
        Station m_arrivalStation;
        QString m_timeInfo;
};


class JourneyDetailResultItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(StopStation departureStation READ departureStation WRITE setDepartureStation)
    Q_PROPERTY(QString departureInfo READ departureInfo WRITE setDepartureInfo)
    Q_PROPERTY(StopStation arrivalStation READ arrivalStation WRITE setArrivalStation)
    Q_PROPERTY(QString arrivalInfo READ arrivalInfo WRITE setArrivalInfo)
    Q_PROPERTY(QString info READ info WRITE setInfo)
    Q_PROPERTY(QString train READ train WRITE setTrain)

    //Some Internal Data fields, primarly to store additional data per backend, like the details url
    Q_PROPERTY(QString internalData1 READ internalData1 WRITE setInternalData1)
    Q_PROPERTY(QString internalData2 READ internalData2 WRITE setInternalData2)
    public:
        StopStation departureStation() const;
        void setDepartureStation(const StopStation &);
        QString departureInfo() const;
        void setDepartureInfo(const QString &);
        StopStation arrivalStation() const;
        void setArrivalStation(const StopStation &);
        QString arrivalInfo() const;
        void setArrivalInfo(const QString &);
        QString info() const;
        void setInfo(const QString &);
        QString train() const;
        void setTrain(const QString &);
        QString internalData1() const;
        void setInternalData1(const QString &);
        QString internalData2() const;
        void setInternalData2(const QString &);
    private:
        StopStation m_departureStation;
        QString m_departureInfo;
        StopStation m_arrivalStation;
        QString m_arrivalInfo;
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
    Q_PROPERTY(StopStation departureStation READ departureStation WRITE setDepartureStation)
    Q_PROPERTY(StopStation viaStation READ viaStation WRITE setViaStation)
    Q_PROPERTY(StopStation arrivalStation READ arrivalStation WRITE setArrivalStation)

    public slots:
        JourneyDetailResultItem *getItem(int);
    public:
        void appendItem(JourneyDetailResultItem *item);
        qreal itemcount();
        QString id() const;
        void setId(const QString &);
        StopStation departureStation() const;
        void setDepartureStation(const StopStation &);
        StopStation viaStation() const;
        void setViaStation(const StopStation &);
        StopStation arrivalStation() const;
        void setArrivalStation(const StopStation &);
        QString info() const;
        void setInfo(const QString &);
        QString duration() const;
        void setDuration(const QString &);
    private:
        QList<JourneyDetailResultItem*> m_items;
        QString m_id;
        StopStation m_departureStation;
        StopStation m_viaStation;
        StopStation m_arrivalStation;
        QString m_info;
        QString m_duration;
};

#endif // PARSER_DEFINITIONS_H
