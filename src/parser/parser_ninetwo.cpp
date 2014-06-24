#include "parser_ninetwo.h"

#include <QtCore/QUrl>
//for backwards compatibility with qt4
#ifndef BUILD_FOR_QT5
//for QUrl/QUrlQuery difference
#define setQuery(q) setQueryItems(q.queryItems())
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
    QJsonObject doc = QJsonDocument::fromJson(allData).object();
    qDebug() << "REPLY:>>>>>>>>>>>>\n" << allData;
    QJsonArray tabs = doc["tabs"].toArray();
    QString currentStation(doc["location"].toObject()["name"].toString());

    for(int i=0; i<tabs.size(); i++){
        QJsonObject tab= tabs[i].toObject();
        QString type=tab["id"].toString();
        QJsonArray departures = tab["departures"].toArray();
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

        for(int j=0; j<departures.size(); j++){
            QJsonObject departure=departures[j].toObject();
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
            entry.trainType = departure["mode"].toObject()["name"].toString();
            result.append(entry);
        }
    }
    emit timetableResult(result);

}

void ParserNinetwo::parseStationsByName(QNetworkReply *networkReply)
{
    qDebug() << "PARSING STATIONS";
    QByteArray allData = networkReply->readAll();
    QJsonObject doc = QJsonDocument::fromJson(allData).object();
  //  qDebug() << "REPLY:>>>>>>>>>>>>\n" << allData;
    QJsonArray stations = doc["locations"].toArray();

    StationsList result;

    for(int i=0; i<stations.size(); i++){
        QJsonObject station=stations[i].toObject();
        Station s;
        s.latitude=station["latLong"].toObject()["lat"].toDouble();
        s.longitude=station["latLong"].toObject()["long"].toDouble();
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
    QJsonObject doc = QJsonDocument::fromJson(allData).object();
    qDebug() << "REPLY:>>>>>>>>>>>>\n" << allData;
    QJsonArray journeys = doc["journeys"].toArray();

    JourneyResultList* result=new JourneyResultList;

    QDateTime arrival;
    QDateTime departure;

    for(int i=0; i<journeys.size(); i++){
        QJsonObject journey=journeys[i].toObject();
        parseJourneyOption(journey);
        JourneyResultItem* item = new JourneyResultItem;
        arrival  = QDateTime::fromString(journey["arrival"  ].toString(), "yyyy-MM-ddTHH:mm");
        departure= QDateTime::fromString(journey["departure"].toString(), "yyyy-MM-ddTHH:mm");
        if(!i)
            lastsearch.firstOption=departure;

        item->setArrivalTime(arrival.toString("HH:mm"));
        item->setDepartureTime(departure.toString("HH:mm"));

        QJsonArray legs = journey["legs"].toArray();
        QStringList trains;
        for(int i=0; i<legs.size(); i++)
        {
            QJsonObject leg=legs[i].toObject();
            QString typeName=leg["mode"].toObject()["name"].toString();
            QString type=leg["mode"].toObject()["type"].toString();

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

void ParserNinetwo::parseJourneyOption(QJsonObject object)
{
    JourneyDetailResultList* result = new JourneyDetailResultList;
    QString id=object["id"].toString();
    QJsonArray legs = object["legs"].toArray();

    QDateTime arrival  = QDateTime::fromString(object["arrival"  ].toString(), "yyyy-MM-ddTHH:mm");
    QDateTime departure  = QDateTime::fromString(object["departure"  ].toString(), "yyyy-MM-ddTHH:mm");
    result->setArrivalDateTime(arrival);
    result->setDepartureDateTime(departure);
    int minutes=departure.secsTo(arrival)/60;
    int hours=minutes/60;
    minutes=minutes%60;
    result->setDuration(QString("%1:%2").arg(hours).arg(minutes, 2, 10, QChar('0')));
    result->setId(id);
    for(int i=0; i<legs.size(); i++)
    {
        QJsonObject leg=legs[i].toObject();
        JourneyDetailResultItem* resultItem = new JourneyDetailResultItem;


        QJsonArray stops = leg["stops"].toArray();
        QJsonObject firstStop = stops[0].toObject();
        QJsonObject lastStop = stops[stops.size()-1].toObject();
        QJsonObject firstLocation = firstStop["location"].toObject();
        QJsonObject lastLocation = lastStop["location"].toObject();


        resultItem->setArrivalStation(lastLocation["name"].toString());
        resultItem->setDepartureStation(firstLocation["name"].toString());

        QDateTime stopDeparture  = QDateTime::fromString(firstStop["departure"].toString(), "yyyy-MM-ddTHH:mm");
        QDateTime stopArrival  = QDateTime::fromString(lastStop["arrival"].toString(), "yyyy-MM-ddTHH:mm");

        resultItem->setDepartureDateTime(stopDeparture);
        resultItem->setArrivalDateTime(stopArrival);

        QString type=leg["mode"].toObject()["type"].toString();
        QString typeName=leg["mode"].toObject()["name"].toString();

        //Fallback if typeName is empty
        if (typeName.length() == 0) {
            typeName = type;
        }

        if(type=="bus" || type=="tram" || type=="train" || type=="subway"){
            if (firstStop["platform"].toString().length() > 0) {
                resultItem->setDepartureInfo(tr("Pl.") + " " + firstStop["platform"].toString());
            }
            if (lastStop["platform"].toString() > 0) {
                resultItem->setArrivalInfo(tr("Pl.") + " " + lastStop["platform"].toString());
            }
            resultItem->setTrain(tr("%1 to %2").arg(typeName).arg(leg["destination"].toString()));
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
