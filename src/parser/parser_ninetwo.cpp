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

#define BASE_URL "https://api.9292.nl/0.1/"

ParserNinetwo::ParserNinetwo(QObject *parent):ParserAbstract(parent)
{
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
    query.addQueryItem("latlong", QString("%1,%2").arg(latitude).arg(longitude));
    uri.setQuery(query);

    sendHttpRequest(uri);
    currentRequestState=FahrplanNS::stationsByCoordinatesRequest;
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
 restrictions << tr("not by ferry");
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
            if (via != "" && !via.isNull())
                via = tr("via %1");
            QString remark(departure.value("remark").toString());
            if (departure.value("realtimeState").toString() == "late") { //it is delayed
                QString rtMessage(departure.value("realtimeText").toString());
                entry.miscInfo=QString(tr("(%1) %2 \n%3").arg(rtMessage, via, remark)).trimmed();
            }
            else
                entry.miscInfo=QString("%1 %2").arg(via, remark).trimmed();

            entry.platform = departure.value("platform").toString();
            entry.trainType = departure.value("mode").toMap().value("name").toString();
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
        s.latitude = station.value("latLong").toMap().value("lat").toDouble();
        s.longitude = station.value("latLong").toMap().value("long").toDouble();
        //s.name=QString("[%1]%2").arg(station.value("type").toString(), station.value("name").toString());
        s.name = station.value("name").toString();
        s.miscInfo = station.value("type").toString();
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
            QVariantMap leg = j->toMap();
            QString typeName = leg.value("mode").toMap().value("name").toString();
            QString type = leg.value("mode").toMap().value("type").toString();

            if(type=="bus" || type=="tram" || type=="train" || type=="subway"){
                if (typeName.length() > 0) {
                    trains.append(typeName);
                }
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
        QString typeName = leg.value("mode").toMap().value("name").toString();

        //Fallback if typeName is empty
        if (typeName.length() == 0) {
            typeName = type;
        }

        if(type=="bus" || type=="tram" || type=="train" || type=="subway"){
            if (firstStop.value("platform").toString().length() > 0) {
                resultItem->setDepartureInfo(tr("Pl. %1")
                                             .arg(firstStop.value("platform").toString()));
            }
            if (lastStop.value("platform").toString() > 0) {
                resultItem->setArrivalInfo(tr("Pl. %1").arg(lastStop.value("platform").toString()));
            }
            resultItem->setTrain(typeName);
            resultItem->setDirection(leg.value("destination").toString());
        }
        else{
            resultItem->setTrain(type);
        }

        result->appendItem(resultItem);
    }

    result->setDepartureStation(result->getItem(0)->departureStation());
    result->setArrivalStation(result->getItem(result->itemcount() - 1)->arrivalStation());

    cachedResults.insert(id, result);
}
