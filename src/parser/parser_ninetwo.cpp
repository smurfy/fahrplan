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

#include "parser_ninetwo.h"

#include <QUrl>
#include <QNetworkReply>
#ifdef BUILD_FOR_QT5
#   include <QUrlQuery>
#else
#   define setQuery(q) setQueryItems(q.queryItems())
#endif
#include <qmath.h>

#define BASE_URL "https://api.9292.nl/0.1/"

inline qreal deg2rad(qreal deg)
{
    return deg * 3.141592653589793238463 / 180;
}

inline int distance(qreal lat1, qreal lon1, qreal lat2, qreal lon2)
{
    const qreal sdLat = qSin(deg2rad(lat2 - lat1) / 2);
    const qreal sdLon = qSin(deg2rad(lon2 - lon1) / 2);
    const qreal cLat1 = qCos(deg2rad(lat1));
    const qreal cLat2 = qCos(deg2rad(lat2));
    const qreal a = sdLat * sdLat + sdLon * sdLon * cLat1 * cLat2;
    const qreal c = 2 * qAtan2(qSqrt(a), qSqrt(1 - a));

    // 6371 km - Earth radius.
    // We return distance in meters, thus * 1000.
    return qRound(6371.0 * c * 1000);
}

ParserNinetwo::ParserNinetwo(QObject *parent):ParserAbstract(parent)
{
    lastCoordinates.isValid = false;
}

void ParserNinetwo::getTimeTableForStation(const Station &currentStation,
                                           const Station &,
                                           const QDateTime &,
                                           ParserAbstract::Mode,
                                           int trainrestrictions)
{
    QUrl uri(QString(BASE_URL "/locations/%1/departure-times").arg(currentStation.id.toString()));

#if defined(BUILD_FOR_QT5)
    QUrlQuery query;
#else
    QUrl query;
#endif

    query.addQueryItem("lang", 	"en-GB");
    uri.setQuery(query);

    timetableRestrictions  =  trainrestrictions;
    sendHttpRequest(uri);
    currentRequestState=FahrplanNS::getTimeTableForStationRequest;
}

void ParserNinetwo::findStationsByName(const QString &stationName)
{
    qDebug() << "FINDBYNAME";
    QUrl uri(BASE_URL "locations");

#if defined(BUILD_FOR_QT5)
    QUrlQuery query;
#else
    QUrl query;
#endif
    query.addQueryItem("lang", 	"en-GB");
    query.addQueryItem("q", stationName);
    uri.setQuery(query);

    sendHttpRequest(uri);
    currentRequestState=FahrplanNS::stationsByNameRequest;
}

void ParserNinetwo::findStationsByCoordinates(qreal longitude, qreal latitude)
{

    QUrl uri(BASE_URL "locations");

#if defined(BUILD_FOR_QT5)
    QUrlQuery query;
#else
    QUrl query;
#endif
    query.addQueryItem("lang", 	"en-GB");
    query.addQueryItem("type", "station,stop");
    query.addQueryItem("latlong", QString("%1,%2").arg(latitude).arg(longitude));
    query.addQueryItem("includestation", "true");
    uri.setQuery(query);

    lastCoordinates.isValid = true;
    lastCoordinates.latitude = latitude;
    lastCoordinates.longitude = longitude;
    currentRequestState = FahrplanNS::stationsByCoordinatesRequest;

    sendHttpRequest(uri);
}

void ParserNinetwo::searchJourney(const Station &departureStation,
                                  const Station &viaStation,
                                  const Station &arrivalStation,
                                  const QDateTime &dateTime,
                                  const ParserAbstract::Mode mode,
                                  int trainrestrictions)
{
    lastsearch.from=departureStation;
    lastsearch.to=arrivalStation;
    lastsearch.restrictions=trainrestrictions;
    lastsearch.via=viaStation;
    QUrl uri(BASE_URL "journeys");

#if defined(BUILD_FOR_QT5)
    QUrlQuery query;
#else
    QUrl query;
#endif
    query.addQueryItem("lang", 	"en-GB");
    query.addQueryItem("before", "1");
    query.addQueryItem("from", departureStation.id.toString());
    if(viaStation.valid)
        query.addQueryItem("via", viaStation.id.toString());
    query.addQueryItem("to", arrivalStation.id.toString());
    query.addQueryItem("sequence", "1");
    query.addQueryItem("dateTime", dateTime.toString("yyyy-MM-ddTHHmm"));

    switch(trainrestrictions){
    default:
    case all:
        query.addQueryItem("byFerry", "true");
    case noFerry:
        query.addQueryItem("byBus", "true");
        query.addQueryItem("byTram", "true");
        query.addQueryItem("bySubway", "true");
    case trainsOnly:
        query.addQueryItem("byTrain", "true");
    }
    query.addQueryItem("searchtype", mode==Departure?"departure":"arrival");

    //TODO: make transport types work
    query.addQueryItem("after", "5");

    uri.setQuery(query);
    sendHttpRequest(uri);

    currentRequestState=FahrplanNS::searchJourneyRequest;

}

void ParserNinetwo::searchJourneyLater()
{
    searchJourney(lastsearch.from, lastsearch.via, lastsearch.to, lastsearch.lastOption, Departure , lastsearch.restrictions);

}

void ParserNinetwo::searchJourneyEarlier()
{
    QDateTime time = lastsearch.firstOption.addSecs(-30*60);
    searchJourney(lastsearch.from, lastsearch.via, lastsearch.to,time, Departure, lastsearch.restrictions);
}

void ParserNinetwo::getJourneyDetails(const QString &id)
{
    if(cachedResults.contains(id))
        emit journeyDetailsResult(cachedResults.value(id));
}

QStringList ParserNinetwo::getTrainRestrictions()
{
 QStringList restrictions;
 restrictions << tr("All");
 restrictions << tr("Only trains");
 restrictions << tr("All, except ferry");
 return restrictions;
}

void ParserNinetwo::parseTimeTable(QNetworkReply *networkReply)
{
    TimetableEntriesList result;
    QByteArray allData = networkReply->readAll();
    qDebug() << "REPLY:>>>>>>>>>>>>\n" << allData;

    QVariantMap doc = parseJson(allData);
    if (doc.isEmpty()) {
        emit errorOccured(tr("Cannot parse reply from the server"));
        return;
    }

    QVariantList tabs = doc.value("tabs").toList();
    QString currentStation(doc.value("location").toMap().value("name").toString());

    QVariantList::const_iterator i;
    for (i = tabs.constBegin(); i != tabs.constEnd(); ++i) {
        QVariantMap tab = i->toMap();
        QString type = tab.value("id").toString();
        QVariantList departures = tab.value("departures").toList();
        switch(timetableRestrictions){
            case all:
            default:
            break;
        case trainsOnly:
            if(type!="train")
                continue;
            break;
        case noFerry:
            if(type=="ferry")//not sure if this ever happens
                continue;
            break;
        }

        QVariantList::const_iterator j;
        for (j = departures.constBegin(); j != departures.constEnd(); ++j) {
            QVariantMap departure = j->toMap();
            TimetableEntry entry;
            entry.currentStation=currentStation;
            entry.destinationStation = departure.value("destinationName").toString();
            entry.time = QTime::fromString(departure.value("time").toString(), "HH:mm");
            QString via(departure.value("viaNames").toString());
            if (!via.isEmpty())
                entry.destinationStation = tr("%1 via %2").arg(entry.destinationStation, via);
            QStringList info;
            const QString rtStatus = departure.value("realtimeState").toString();
            if (rtStatus == "ontime") {
                info << tr("On-Time");
            } else if (rtStatus == "late") {
                const QString rtMessage = departure.value("realtimeText").toString().trimmed();
                if (!rtMessage.isEmpty())
                    info << QString("<span style=\"color:#b30;\">%1</span>").arg(rtMessage);
            }
            const QString remark = departure.value("remark").toString();
            if (!remark.isEmpty())
                info << remark;
            entry.miscInfo = info.join("<br />");

            entry.platform = departure.value("platform").toString();

            QString train = departure.value("mode").toMap().value("name").toString();
            const QString service = departure.value("service").toString();
            if (!service.isEmpty())
                train.append(" ").append(service);
            entry.trainType = train;

            result.append(entry);
        }
    }
    emit timetableResult(result);

}

void ParserNinetwo::parseStationsByName(QNetworkReply *networkReply)
{
    qDebug() << "PARSING STATIONS";
    QByteArray allData = networkReply->readAll();
//    qDebug() << "REPLY:>>>>>>>>>>>>\n" << allData;

    QVariantMap doc = parseJson(allData);
    if (doc.isEmpty()) {
        emit errorOccured(tr("Cannot parse reply from the server"));
        return;
    }

    QVariantList stations = doc.value("locations").toList();

    StationsList result;

    QVariantList::const_iterator i;
    for (i = stations.constBegin(); i != stations.constEnd(); ++i) {
        QVariantMap station = i->toMap();
        Station s;
        bool okLat, okLon;
        double lat = station.value("latLong").toMap().value("lat").toDouble(&okLat);
        double lon = station.value("latLong").toMap().value("long").toDouble(&okLon);
        //s.name=QString("[%1]%2").arg(station.value("type").toString(), station.value("name").toString());
        s.name = station.value("name").toString();
        s.miscInfo = station.value("type").toString();
        if (okLat && okLon) {
            s.latitude = lat;
            s.longitude = lon;
            if (lastCoordinates.isValid) {
                if (!s.miscInfo.isEmpty())
                    s.miscInfo.prepend(", ");
                //: Distance in meters
                s.miscInfo.prepend(tr("%n m", "",
                                      distance(lastCoordinates.latitude, lastCoordinates.longitude,
                                               s.latitude, s.longitude)));
            }
        }
        if (station.value("type").toString() == "address")
            s.name = s.name + " " + station.value("houseNr").toString();
        s.id = station.value("id").toString();
        result.append(s);
    }

    emit stationsResult(result);
}

void ParserNinetwo::parseStationsByCoordinates(QNetworkReply *networkReply)
{
    parseStationsByName(networkReply);

    lastCoordinates.isValid = false;
}

void ParserNinetwo::parseSearchJourney(QNetworkReply *networkReply)
{
    qDebug() << "PARSING JOURNEYS";
    QByteArray allData = networkReply->readAll();
    qDebug() << "REPLY:>>>>>>>>>>>>\n" << allData;

    QVariantMap doc = parseJson(allData);
    if (doc.isEmpty()) {
        emit errorOccured(tr("Cannot parse reply from the server"));
        return;
    }

    QVariantList journeys = doc.value("journeys").toList();

    JourneyResultList* result=new JourneyResultList;

    QDateTime arrival;
    QDateTime departure;

    QVariantList::const_iterator i;
    for (i = journeys.constBegin(); i != journeys.constEnd(); ++i) {
        QVariantMap journey = i->toMap();
        parseJourneyOption(journey);
        JourneyResultItem* item = new JourneyResultItem;
        arrival = QDateTime::fromString(journey.value("arrival").toString(), "yyyy-MM-ddTHH:mm");
        departure = QDateTime::fromString(journey.value("departure").toString(),
                                          "yyyy-MM-ddTHH:mm");
        if (i == journeys.constBegin())
            lastsearch.firstOption=departure;

        item->setArrivalTime(arrival.toString("HH:mm"));
        item->setDepartureTime(departure.toString("HH:mm"));

        QVariantList legs = journey.value("legs").toList();
        QStringList trains;

        QVariantList::const_iterator j;
        for (j = legs.constBegin(); j != legs.constEnd(); ++j)
        {
            const QVariantMap mode = j->toMap().value("mode").toMap();
            if (mode.value("type").toString() != "walk") {
                const QString typeName = mode.value("name").toString();
                if (!typeName.isEmpty())
                    trains.append(typeName);
            }
        }

        trains.removeDuplicates();

        item->setTrainType(trains.join(", ").trimmed());
        item->setTransfers(QString::number((int) journey.value("numberOfChanges").toDouble()));
        int minutes = departure.secsTo(arrival)/60;
        item->setDuration(QString("%1:%2").arg(minutes/60).arg(minutes%60,2,10,QChar('0')));
        item->setId(journey.value("id").toString());
        result->appendItem(item);

        //Set result metadata based on first result
        if (result->itemcount() == 1) {
            result->setTimeInfo(arrival.date().toString());
            result->setDepartureStation(cachedResults[item->id()]->departureStation());
            result->setArrivalStation(cachedResults[item->id()]->arrivalStation());
        }
    }
    lastsearch.lastOption=departure;
    emit journeyResult(result);
}

void ParserNinetwo::parseSearchLaterJourney(QNetworkReply *)
{

}

void ParserNinetwo::parseSearchEarlierJourney(QNetworkReply *)
{

}

void ParserNinetwo::parseJourneyDetails(QNetworkReply *)
{
    //should never happen
}

void ParserNinetwo::parseJourneyOption(const QVariantMap &object)
{
    JourneyDetailResultList* result = new JourneyDetailResultList;
    QString id = object.value("id").toString();
    QVariantList legs = object.value("legs").toList();

    QDateTime arrival = QDateTime::fromString(object.value("arrival").toString(),
                                              "yyyy-MM-ddTHH:mm");
    QDateTime departure = QDateTime::fromString(object.value("departure").toString(),
                                                "yyyy-MM-ddTHH:mm");
    result->setArrivalDateTime(arrival);
    result->setDepartureDateTime(departure);
    int minutes=departure.secsTo(arrival)/60;
    int hours=minutes/60;
    minutes=minutes%60;
    result->setDuration(QString("%1:%2").arg(hours).arg(minutes, 2, 10, QChar('0')));
    result->setId(id);
    for(int i = 0; i < legs.count(); i++)
    {
        QVariantMap leg = legs.at(i).toMap();
        JourneyDetailResultItem* resultItem = new JourneyDetailResultItem;


        QVariantList stops = leg.value("stops").toList();
        QVariantMap firstStop = stops[0].toMap();
        QVariantMap lastStop = stops[stops.size()-1].toMap();
        QVariantMap firstLocation = firstStop.value("location").toMap();
        QVariantMap lastLocation = lastStop.value("location").toMap();


        resultItem->setArrivalStation(lastLocation.value("name").toString());
        resultItem->setDepartureStation(firstLocation.value("name").toString());

        QDateTime stopDeparture = QDateTime::fromString(firstStop.value("departure").toString(),
                                                        "yyyy-MM-ddTHH:mm");
        QDateTime stopArrival = QDateTime::fromString(lastStop.value("arrival").toString(),
                                                      "yyyy-MM-ddTHH:mm");

        resultItem->setDepartureDateTime(stopDeparture);
        resultItem->setArrivalDateTime(stopArrival);

        QString type = leg.value("mode").toMap().value("type").toString();
        QString typeName;
        if (type != "walk")
            typeName = leg.value("mode").toMap().value("name").toString();

        //Fallback if typeName is empty
        if (typeName.isEmpty() && !type.isEmpty()) {
            typeName = type;
            typeName[0] = type.at(0).toUpper();
        }

        const QString service = leg.value("service").toString();
        if (!service.isEmpty())
            typeName.append(" ").append(service);

        if (type == "walk") {
            const QString duration = leg.value("duration").toString();
            resultItem->setTrain(tr("%1 for %2 min").arg(typeName, duration));
        } else {
            resultItem->setTrain(typeName);
        }

        if (!firstStop.value("platform").toString().isEmpty()) {
            resultItem->setDepartureInfo(tr("Pl. %1")
                                         .arg(firstStop.value("platform").toString()));
        }
        if (!lastStop.value("platform").toString().isEmpty()) {
            resultItem->setArrivalInfo(tr("Pl. %1").arg(lastStop.value("platform").toString()));
        }
        resultItem->setDirection(leg.value("destination").toString());

        QStringList attributes;
        const QVariantList attrs = leg.value("attributes").toList();
        foreach (const QVariant &attr, attrs) {
            attributes << attr.toMap().value("title").toString();
        }
        resultItem->setInfo(attributes.join(tr(", ")));

        result->appendItem(resultItem);
    }

    result->setDepartureStation(result->getItem(0)->departureStation());
    result->setArrivalStation(result->getItem(result->itemcount() - 1)->arrivalStation());

    cachedResults.insert(id, result);
}
