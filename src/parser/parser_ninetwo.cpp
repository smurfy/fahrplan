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
#ifdef BUILD_FOR_QT5
#   include <QUrlQuery>
#   include <QJsonArray>
#   include <QJsonDocument>
#   include <QJsonObject>
#else
#   include <QScriptEngine>
#   include <QScriptValue>
#   define setQuery(q) setQueryItems(q.queryItems())
#endif

#define BASE_URL "https://api.9292.nl/0.1/"

ParserNinetwo::ParserNinetwo(QObject *parent):ParserAbstract(parent)
{
}

void ParserNinetwo::getTimeTableForStation(const Station &currentStation, const Station &directionStation, const QDateTime &dateTtime, ParserAbstract::Mode mode, int trainrestrictions)
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

void ParserNinetwo::searchJourney(const Station &departureStation,const Station &viaStation,const Station &arrivalStation,const QDateTime &dateTime,const ParserAbstract::Mode mode, int trainrestrictions)
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
        emit journeyDetailsResult(cachedResults[id]);
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

    QVariantList tabs = doc["tabs"].toList();
    QString currentStation(doc["location"].toMap()["name"].toString());

    for (int i = 0; i < tabs.count(); i++) {
        QVariantMap tab = tabs[i].toMap();
        QString type=tab["id"].toString();
        QVariantList departures = tab["departures"].toList();
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

        for (int j = 0; j < departures.count(); j++) {
            QVariantMap departure = departures[j].toMap();
            TimetableEntry entry;
            entry.currentStation=currentStation;
            entry.destinationStation=departure["destinationName"].toString();
            entry.time=QTime::fromString(departure["time"].toString(), "HH:mm");
            QString via(departure["viaNames"].toString());
            if(via!="" && !via.isNull())
                via= tr("via ") + via;
            QString remark(departure["remark"].toString());
            if(departure["realtimeState"].toString()=="late"){//it is delayed
                QString rtMessage(departure["realtimeText"].toString());
                entry.miscInfo=QString(tr("(%1) %2 \n%3").arg(rtMessage, via, remark)).trimmed();
            }
            else
                entry.miscInfo=QString("%1 %2").arg(via, remark).trimmed();

            entry.platform=departure["platform"].toString();
            entry.trainType = departure["mode"].toMap()["name"].toString();
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

    QVariantList stations = doc["locations"].toList();

    StationsList result;

    for (int i = 0; i < stations.count(); i++) {
        QVariantMap station = stations[i].toMap();
        Station s;
        s.latitude = station["latLong"].toMap()["lat"].toDouble();
        s.longitude = station["latLong"].toMap()["long"].toDouble();
        //s.name=QString("[%1]%2").arg(station["type"].toString(), station["name"].toString());
        s.name=station["name"].toString();
        s.miscInfo=station["type"].toString();
        if(QString("address") == station["type"].toString())
            s.name = s.name + " " + station["houseNr"].toString();
        s.id=station["id"].toString();
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

    QVariantList journeys = doc["journeys"].toList();

    JourneyResultList* result=new JourneyResultList;

    QDateTime arrival;
    QDateTime departure;

    for (int i = 0; i < journeys.count(); i++) {
        QVariantMap journey = journeys[i].toMap();
        parseJourneyOption(journey);
        JourneyResultItem* item = new JourneyResultItem;
        arrival  = QDateTime::fromString(journey["arrival"  ].toString(), "yyyy-MM-ddTHH:mm");
        departure= QDateTime::fromString(journey["departure"].toString(), "yyyy-MM-ddTHH:mm");
        if(!i)
            lastsearch.firstOption=departure;

        item->setArrivalTime(arrival.toString("HH:mm"));
        item->setDepartureTime(departure.toString("HH:mm"));

        QVariantList legs = journey["legs"].toList();
        QStringList trains;
        for (int i = 0; i < legs.count(); i++)
        {
            QVariantMap leg = legs[i].toMap();
            QString typeName = leg["mode"].toMap()["name"].toString();
            QString type = leg["mode"].toMap()["type"].toString();

            if(type=="bus" || type=="tram" || type=="train" || type=="subway"){
                if (typeName.length() > 0) {
                    trains.append(typeName);
                }
            }
        }

        trains.removeDuplicates();

        item->setTrainType(trains.join(", ").trimmed());
        item->setTransfers(QString::number((int) journey["numberOfChanges"].toDouble()));
        int minutes = departure.secsTo(arrival)/60;
        item->setDuration(QString("%1:%2").arg(minutes/60).arg(minutes%60,2,10,QChar('0')));
        item->setId(journey["id"].toString());
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

void ParserNinetwo::parseSearchLaterJourney(QNetworkReply *networkReply)
{

}

void ParserNinetwo::parseSearchEarlierJourney(QNetworkReply *networkReply)
{

}

void ParserNinetwo::parseJourneyDetails(QNetworkReply *networkReply)
{
    //should never happen
}

QVariantMap ParserNinetwo::parseJson(const QByteArray &json) const
{
    QVariantMap doc;
#ifdef BUILD_FOR_QT5
    doc = QJsonDocument::fromJson(json).toVariant().toMap();
#else
    QString tmp(json);
    // Validation of JSON according to RFC4627, section 6
    if (tmp.replace(QRegExp("\"(\\\\.|[^\"\\\\])*\""), "")
           .contains(QRegExp("[^,:{}\\[\\]0-9.\\-+Eaeflnr-u \\n\\r\\t]")))
        return doc;

    QScriptEngine *engine = new QScriptEngine();
    doc = engine->evaluate("(" + json + ")").toVariant().toMap();
    delete engine;
#endif

    return doc;
}

void ParserNinetwo::parseJourneyOption(const QVariantMap &object)
{
    JourneyDetailResultList* result = new JourneyDetailResultList;
    QString id=object["id"].toString();
    QVariantList legs = object["legs"].toList();

    QDateTime arrival  = QDateTime::fromString(object["arrival"  ].toString(), "yyyy-MM-ddTHH:mm");
    QDateTime departure  = QDateTime::fromString(object["departure"  ].toString(), "yyyy-MM-ddTHH:mm");
    result->setArrivalDateTime(arrival);
    result->setDepartureDateTime(departure);
    int minutes=departure.secsTo(arrival)/60;
    int hours=minutes/60;
    minutes=minutes%60;
    result->setDuration(QString("%1:%2").arg(hours).arg(minutes, 2, 10, QChar('0')));
    result->setId(id);
    for(int i = 0; i < legs.count(); i++)
    {
        QVariantMap leg = legs[i].toMap();
        JourneyDetailResultItem* resultItem = new JourneyDetailResultItem;


        QVariantList stops = leg["stops"].toList();
        QVariantMap firstStop = stops[0].toMap();
        QVariantMap lastStop = stops[stops.size()-1].toMap();
        QVariantMap firstLocation = firstStop["location"].toMap();
        QVariantMap lastLocation = lastStop["location"].toMap();


        resultItem->setArrivalStation(lastLocation["name"].toString());
        resultItem->setDepartureStation(firstLocation["name"].toString());

        QDateTime stopDeparture  = QDateTime::fromString(firstStop["departure"].toString(), "yyyy-MM-ddTHH:mm");
        QDateTime stopArrival  = QDateTime::fromString(lastStop["arrival"].toString(), "yyyy-MM-ddTHH:mm");

        resultItem->setDepartureDateTime(stopDeparture);
        resultItem->setArrivalDateTime(stopArrival);

        QString type = leg["mode"].toMap()["type"].toString();
        QString typeName = leg["mode"].toMap()["name"].toString();

        //Fallback if typeName is empty
        if (typeName.length() == 0) {
            typeName = type;
        }

        if(type=="bus" || type=="tram" || type=="train" || type=="subway"){
            if (firstStop["platform"].toString().length() > 0) {
                resultItem->setDepartureInfo(tr("Pl. %1").arg(firstStop["platform"].toString()));
            }
            if (lastStop["platform"].toString() > 0) {
                resultItem->setArrivalInfo(tr("Pl. %1").arg(lastStop["platform"].toString()));
            }
            resultItem->setTrain(typeName);
            resultItem->setDirection(leg["destination"].toString());
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
