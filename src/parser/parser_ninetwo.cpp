#include "parser_ninetwo.h"

#define BASE_URL "https://api.9292.nl/0.1/"

ParserNinetwo::ParserNinetwo(QObject *parent):ParserAbstract(parent)
{
}

void ParserNinetwo::getTimeTableForStation(Station &currentStation, Station &directionStation, QDateTime &dateTtime, ParserAbstract::Mode mode, int trainrestrictions)
{

}

void ParserNinetwo::findStationsByName(const QString &stationName)
{
    qDebug() << "FINDBYNAME";
    QUrl uri(BASE_URL "locations");

    QUrlQuery query;
    query.addQueryItem("lang", 	"en-GB");
    query.addQueryItem("q", stationName);
    uri.setQuery(query);

    sendHttpRequest(uri);
    currentRequestState=FahrplanNS::stationsByNameRequest;
    qDebug() << uri.url();
}

void ParserNinetwo::findStationsByCoordinates(qreal longitude, qreal latitude)
{

    QUrl uri(BASE_URL "locations");

    QUrlQuery query;
    query.addQueryItem("lang", 	"en-GB");
    query.addQueryItem("latlong", QString("%1,%2").arg(latitude).arg(longitude));
    uri.setQuery(query);
    qDebug() << uri.url();

    sendHttpRequest(uri);
    currentRequestState=FahrplanNS::stationsByCoordinatesRequest;
}

void ParserNinetwo::searchJourney(const Station &departureStation,const Station &viaStation,const Station &arrivalStation,const QDateTime &dateTime,const ParserAbstract::Mode mode, int trainrestrictions)
{
    QUrl uri(BASE_URL "journeys");

    QUrlQuery query;
    query.addQueryItem("lang", 	"en-GB");
    query.addQueryItem("before", "1");
    query.addQueryItem("from", departureStation.id.toString());
    if(viaStation.valid)
        query.addQueryItem("via", viaStation.id.toString());
    query.addQueryItem("to", arrivalStation.id.toString());
    query.addQueryItem("sequence", "1");
    query.addQueryItem("dateTime", dateTime.toString("yyyy-MM-ddTHHmm"));
    query.addQueryItem("byFerry", "true");
    query.addQueryItem("byBus", "true");
    query.addQueryItem("byTram", "true");
    query.addQueryItem("bySubway", "true");
    query.addQueryItem("byTrain", "true");

    //TODO: make transport types work
    query.addQueryItem("after", "5");

    uri.setQuery(query);
    sendHttpRequest(uri);

    qDebug() << uri.url();
    currentRequestState=FahrplanNS::searchJourneyRequest;

}

void ParserNinetwo::searchJourneyLater()
{
}

void ParserNinetwo::searchJourneyEarlier()
{
}

void ParserNinetwo::getJourneyDetails(const QString &id)
{
    if(cachedResults.contains(id))
        emit journeyDetailsResult(cachedResults[id]);
}

bool ParserNinetwo::supportsGps()
{
    return true;
}

bool ParserNinetwo::supportsVia()
{
    return true;
}

bool ParserNinetwo::supportsTimeTable()
{
    return true;
}

bool ParserNinetwo::supportsTimeTableDirection()
{
    return false;
}

QStringList ParserNinetwo::getTrainRestrictions()
{
 QStringList restrictions;
 restrictions << tr("All methods");
 restrictions << tr("Only trains");
 restrictions << tr("not by ferry");
 return restrictions;
}

void ParserNinetwo::parseTimeTable(QNetworkReply *networkReply)
{

}

void ParserNinetwo::parseStationsByName(QNetworkReply *networkReply)
{
    qDebug() << "PARSING STATIONS";
    QByteArray allData = networkReply->readAll();
    QJsonObject doc = QJsonDocument::fromJson(allData).object();
    qDebug() << "REPLY:>>>>>>>>>>>>\n" << allData;
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
    for(int i=0; i<journeys.size(); i++){
        QJsonObject journey=journeys[i].toObject();
        parseJourneyOption(journey);
        JourneyResultItem* item = new JourneyResultItem;
        QDateTime arrival  = QDateTime::fromString(journey["arrival"  ].toString(), "yyyy-MM-ddTHH:mm");
        QDateTime departure= QDateTime::fromString(journey["departure"].toString(), "yyyy-MM-ddTHH:mm");

        item->setArrivalTime(departure.toString("HH:mm"));
        item->setDepartureTime(departure.toString("HH:mm"));

        item->setTransfers(QString::number((int) journey["numberOfChanges"].toDouble()));
        int minutes = departure.secsTo(arrival)/60;
        item->setDuration(QString("%1:%2").arg(minutes/60).arg(minutes%60,2,10,QChar('0')));
        item->setId(journey["id"].toString());
        result->appendItem(item);

    }
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

    QDateTime stopDeparture  = QDateTime::fromString(firstStop["departure"  ].toString(), "yyyy-MM-ddTHH:mm");
    QDateTime stopArrival  = QDateTime::fromString(lastStop["arrival"  ].toString(), "yyyy-MM-ddTHH:mm");

        resultItem->setDepartureDateTime(stopDeparture);
        resultItem->setArrivalDateTime(stopArrival);

        QString type=leg["mode"].toObject()["type"].toString();

        if(type=="bus" || type=="tram" || type=="train" || type=="subway"){
            resultItem->setDepartureInfo("platform " + firstStop["platform"].toString());
            resultItem->setArrivalInfo("platform " + lastStop  ["platform"].toString());
            resultItem->setInfo(QString("%1 direction %2").arg(type).arg(leg["destination"].toString()));
        }
        else{
            resultItem->setInfo(type);
        }

        result->appendItem(resultItem);
    }

    cachedResults.insert(id, result);
}
