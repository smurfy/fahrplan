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

#include "parser_london_tfl.h"

#include <QEventLoop>
#include <QUrl>
#include <QNetworkReply>
#ifdef BUILD_FOR_QT5
#   include <QUrlQuery>
#else
#include <QTextDocument>
#   define setQuery(q) setQueryItems(q.queryItems())
#endif
#include <qmath.h>

namespace
{
   const QUrl BaseUrl("https://api.tfl.gov.uk");

   const QString ModeBus = "bus";
   const QString ModeCableCar = "cable-car";
   const QString ModeCoach = "coach";
   const QString ModeDlr = "dlr";
   const QString ModeRail = "national-rail";
   const QString ModeOverground = "overground";
   const QString ModeReplacementBus = "replacement-bus";
   const QString ModeRiverBus = "river-bus";
   const QString ModeRiverTour = "river-tour";
   const QString ModeTflRail = "tflrail";
   const QString ModeTram = "tram";
   const QString ModeTube = "tube";

   bool sortByTimeLessThan(const TimetableEntry &first, const TimetableEntry &second)
   {
       if (first.time == second.time)
           return first.trainType < second.trainType;
       else
           return first.time < second.time;
   }

   QString shortLineName(const QString & lineName)
   {
       QMap<QString,QString> shortNamesMap;

       shortNamesMap["Hammersmith & City"] = "H & C";
       shortNamesMap["London Overground"] = "Overground";

       // not found in map
       if (shortNamesMap.find( lineName ) == shortNamesMap.end())
           return lineName;

       return shortNamesMap[lineName];

   }
}

ParserLondonTfl::ParserLondonTfl(QObject *parent):ParserAbstract(parent)
{
    lastCoordinates.isValid = false;

    NetworkManagerTimeTableSubQuery = new QNetworkAccessManager(this);
}

void ParserLondonTfl::getTimeTableForStation(const Station &currentStation,
                                           const Station &,
                                           const QDateTime &,
                                           ParserAbstract::Mode,
                                           int trainrestrictions)
{
   QUrl relativeUri(QString("/stoppoint/%1").arg(parseJson(currentStation.id.toByteArray()).value("naptanId").toString()));

#if defined(BUILD_FOR_QT5)
    QUrlQuery query;
#else
    QUrl query;
#endif

    relativeUri.setQuery(query);

    timetableRestrictions  =  trainrestrictions;
    sendHttpRequest(BaseUrl.resolved(relativeUri));
    currentRequestState=FahrplanNS::getTimeTableForStationRequest;
}

void ParserLondonTfl::findStationsByName(const QString &stationName)
{
    qDebug() << "FINDBYNAME";
    QUrl relativeUrl (QString("/Stoppoint/Search/%1").arg(stationName));

#if defined(BUILD_FOR_QT5)
    QUrlQuery query;
#else
    QUrl query;
#endif
    relativeUrl.setQuery(query);

    sendHttpRequest(BaseUrl.resolved(relativeUrl));
    currentRequestState=FahrplanNS::stationsByNameRequest;
}

void ParserLondonTfl::findStationsByCoordinates(qreal longitude, qreal latitude)
{
    QUrl relativeuri(QString("/Stoppoint"));

#if defined(BUILD_FOR_QT5)
    QUrlQuery query;
#else
    QUrl query;
#endif

    longitude = -0.13556;
    latitude = 51.52532;

    query.addQueryItem("lon", QString::number(longitude));
    query.addQueryItem("lat", QString::number(latitude));
    query.addQueryItem("stoptypes", "NaptanMetroStation,NaptanRailStation,NaptanBusCoachStation,NaptanFerryPort,NaptanPublicBusCoachTram");

    relativeuri.setQuery(query);

    lastCoordinates.isValid = true;
    lastCoordinates.latitude = latitude;
    lastCoordinates.longitude = longitude;

    sendHttpRequest(BaseUrl.resolved(relativeuri));
    currentRequestState = FahrplanNS::stationsByCoordinatesRequest;
}

void ParserLondonTfl::searchJourney(const Station &departureStation,
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

    QUrl relativeUri(QString("Journey/Journeyresults/%1/to/%2").arg(parseJson(departureStation.id.toByteArray()).value("icsId").toString(), parseJson(arrivalStation.id.toByteArray()).value("icsId").toString()));

#if defined(BUILD_FOR_QT5)
    QUrlQuery query;
#else
    QUrl query;
#endif

    if(viaStation.valid)
        query.addQueryItem("via", parseJson(viaStation.id.toByteArray()).value("icsId").toString());

    query.addQueryItem("date", dateTime.toString("yyyyMMdd"));
    query.addQueryItem("time", dateTime.toString("HHmm"));
    query.addQueryItem("timeIs", (mode == Departure) ? "Departing" : "Arriving");

    QStringList modesList = getModesFromTrainRestrictions(trainrestrictions);

    query.addQueryItem("mode", modesList.join(','));

    /*
    switch(trainrestrictions){
    case all:
        break;
    case bus:
        query.addQueryItem("mode", "bus");
        break;
    case bus_or_tube:
        query.addQueryItem("mode", "bus,tube");
        break;
    }
    */

    relativeUri.setQuery(query);
    sendHttpRequest(BaseUrl.resolved(relativeUri));

    currentRequestState=FahrplanNS::searchJourneyRequest;
}

void ParserLondonTfl::searchJourneyLater()
{
    QDateTime time = lastsearch.firstOption.addSecs(30*60);
    searchJourney(lastsearch.from, lastsearch.via, lastsearch.to, time, Departure , lastsearch.restrictions);

}

void ParserLondonTfl::searchJourneyEarlier()
{
    QDateTime time = lastsearch.firstOption.addSecs(-30*60);
    searchJourney(lastsearch.from, lastsearch.via, lastsearch.to,time, Departure, lastsearch.restrictions);
}

void ParserLondonTfl::getJourneyDetails(const QString &id)
{
    if(cachedResults.contains(id))
        emit journeyDetailsResult(cachedResults.value(id));
}

QStringList ParserLondonTfl::getTrainRestrictions()
{
 QStringList restrictions;
 restrictions << tr("All");
 restrictions << tr("Nat. Rail, Tube, Overground, DLR");
 restrictions << tr("Tube, Overground, DLR");
 restrictions << tr("Bus, Tram, Tube, Overground, DLR");
 restrictions << tr("Bus, Tram");
 restrictions << tr("Bus");
 restrictions << tr("Nat. Rail");
 restrictions << tr("Tube");
 restrictions << tr("Overground");
 restrictions << tr("DLR");

 return restrictions;
}

void ParserLondonTfl::parseTimeTable(QNetworkReply *networkReply)
{
    // the response contains the line groups with the stop points.
    qDebug() << "PARSING STATIONS";
    QByteArray allData = networkReply->readAll();
    qDebug() << "REPLY:>>>>>>>>>>>>\n" << allData;

    QVariantMap doc = parseJson(allData);
    if (doc.isEmpty()) {
        emit errorOccured(tr("Cannot parse reply from the server"));
        return;
    }

    QVariantList lineGroups = doc.value("lineGroup").toList();
    QStringList stopCodes;

    TimetableEntriesList result;
    int counter;

    QVariantList::const_iterator i;
    for (i = lineGroups.begin(); i != lineGroups.constEnd(); ++i) {

        counter++;
        QVariantMap lineGroup = i->toMap();

        QString stopCode;
        stopCode = lineGroup.value("naptanIdReference").toString();

       if (stopCode.isEmpty())
       {
          stopCode = lineGroup.value("stationAtcoCode").toString();
       }

       if (! stopCodes.contains(stopCode))
       {
           stopCodes.append(stopCode);
       }
    }

    QStringList::const_iterator j;
    for (j = stopCodes.constBegin(); j != stopCodes.constEnd(); ++j)
    {
         addTimeTableEntriesOfStopPoint(*j, result);
    }

    // Departures / arrivals are grouped by transportation type,
    // while we want them sorted by departure / arrival time.
    qSort(result.begin(), result.end(), sortByTimeLessThan);

    emit timetableResult(result);
}

void ParserLondonTfl::parseStationsByName(QNetworkReply *networkReply)
{
    qDebug() << "PARSING STATIONS";
    QByteArray allData = networkReply->readAll();
    qDebug() << "REPLY:>>>>>>>>>>>>\n" << allData;

    QVariantMap doc = parseJson(allData);
    if (doc.isEmpty()) {
        emit errorOccured(tr("Cannot parse reply from the server"));
        return;
    }

    QVariantList stations = doc.value("matches").toList();

    StationsList result;

    QVariantList::const_iterator i;
    for (i = stations.constBegin(); i != stations.constEnd(); ++i) {
        QVariantMap station = i->toMap();

        // we need to keep the icsId and the
        QVariantMap idMap;
        idMap["icsId"] = station.value("icsId").toString();
        idMap["naptanId"] = station.value("id").toString();

        Station s;
        s.id = serializeToJson(idMap, false);
        s.name = station.value("name").toString();
        s.latitude = station.value("lat").toDouble();
        s.longitude = station.value("lon").toDouble();
        if (station.contains("zone"))
        {
            s.miscInfo = "Zone " + station.value("zone").toString();
        }
        result.append(s);
    }

    emit stationsResult(result);
}

void ParserLondonTfl::parseStationsByCoordinates(QNetworkReply *networkReply)
{
    qDebug() << "PARSING STATIONS BY COORDINATES";
    QByteArray allData = networkReply->readAll();
    qDebug() << "REPLY:>>>>>>>>>>>>\n" << allData;

    QVariantMap doc = parseJson(allData);
    if (doc.isEmpty()) {
        emit errorOccured(tr("Cannot parse reply from the server"));
        return;
    }

    QStringList icsIds;
    QVariantList stations = doc.value("stopPoints").toList();

    StationsList result;

    QVariantList::const_iterator i;
    for (i = stations.constBegin(); i != stations.constEnd(); ++i) {
        QVariantMap station = i->toMap();

        // we skip entries without icsId
        if (station.value("icsCode").toString().isEmpty())
        {
            continue;
        }

        QString currentIcsId = station.value("icsCode").toString();

        // we skip already found icsIds
        if (icsIds.contains(currentIcsId))
        {
            continue;
        }

        icsIds.append(currentIcsId);

        // we need to keep the icsId and the
        QVariantMap idMap;
        idMap["icsId"] = currentIcsId;
        idMap["naptanId"] = station.value("id").toString();

        Station s;
        s.id = serializeToJson(idMap, false);
        s.name = station.value("commonName").toString();
        s.latitude = station.value("lat").toDouble();
        s.longitude = station.value("lon").toDouble();

        QVariantList additionalProperties = station.value("additionalProperties").toList();

        QVariantList::const_iterator j;
        for (j = additionalProperties.constBegin(); j != additionalProperties.constEnd(); ++j) {

            // find the zone information
            QVariantMap currentProperty = j->toMap();

            if (currentProperty.value("key").toString() == "Zone")
            {
              s.miscInfo = "Zone " + currentProperty.value("value").toString();

              // currently we do not need more info

              break;
            }
        }

        result.append(s);
    }

    emit stationsResult(result);
}

void ParserLondonTfl::parseSearchJourney(QNetworkReply *networkReply)
{
    // baker street to great portland street
    //https://api.tfl.gov.uk/Journey/Journeyresults/1000011/to/1000091

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
    int counter = 0;

    for (i = journeys.constBegin(); i != journeys.constEnd(); ++i) {
        counter++;
        QVariantMap journey = i->toMap();
        QString id = QString::number(counter);
        parseJourneyOption(journey, id);
        JourneyResultItem* item = new JourneyResultItem;
        arrival = QDateTime::fromString(journey.value("arrivalDateTime").toString(), "yyyy-MM-ddTHH:mm:ss");
        departure = QDateTime::fromString(journey.value("startDateTime").toString(), "yyyy-MM-ddTHH:mm:ss");

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
            if (mode.value("type").toString() != "walking") {
                const QString typeName = mode.value("name").toString();
                if (!typeName.isEmpty())
                    trains.append(typeName);
            }
        }

        trains.removeDuplicates();

        item->setTrainType(trains.join(", ").trimmed());

        //item->setTransfers(QString::number((int) journey.value("numberOfChanges").toDouble()));
        item->setTransfers(QString::number(legs.count() - 1));

        int minutes = departure.secsTo(arrival)/60;
        item->setDuration(QString("%1:%2").arg(minutes/60).arg(minutes%60,2,10,QChar('0')));

        //item->setId(journey.value("id").toString());
        //item->setId(randString(10));
        item->setId(id);

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

void ParserLondonTfl::parseSearchLaterJourney(QNetworkReply *)
{

}

void ParserLondonTfl::parseSearchEarlierJourney(QNetworkReply *)
{

}

void ParserLondonTfl::parseJourneyDetails(QNetworkReply *)
{
    //should never happen
}

void ParserLondonTfl::parseJourneyOption(const QVariantMap &object, const QString &id)
{
    JourneyDetailResultList* result = new JourneyDetailResultList;

    QVariantList legs = object.value("legs").toList();

    QDateTime arrival = QDateTime::fromString(object.value("arrivalDateTime").toString(), "yyyy-MM-ddTHH:mm:ss");
    QDateTime departure = QDateTime::fromString(object.value("startDateTime").toString(), "yyyy-MM-ddTHH:mm:ss");
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

        QVariantMap firstStop = leg["departurePoint"].toMap();
        QVariantMap lastStop = leg["arrivalPoint"].toMap();

        resultItem->setDepartureDateTime(QDateTime::fromString(leg.value("departureTime").toString(), "yyyy-MM-ddTHH:mm:ss"));
        resultItem->setArrivalDateTime(QDateTime::fromString(leg.value("arrivalTime").toString(), "yyyy-MM-ddTHH:mm:ss"));
        resultItem->setDepartureStation(firstStop.value("commonName").toString().replace("Underground Station", "🚇"));
        resultItem->setArrivalStation(lastStop.value("commonName").toString().replace("Underground Station", "🚇"));

        QString mode = leg.value("mode").toMap().value("name").toString();

        //const QString service = leg.value("service").toString();
        //if (!service.isEmpty())
        //    typeName.append(" ").append(service);

        if (mode != "walking") {

            // stop letter / platform (departure)
            if (! firstStop.value("stopLetter").toString().isEmpty()) {
                resultItem->setDepartureInfo(QString("🚏 %1").arg(firstStop.value("stopLetter").toString()));
            }

            else if (! firstStop.value("platformName").toString().isEmpty()) {
                resultItem->setDepartureInfo(QString("Pl. %1").arg(firstStop.value("platformName").toString()));
            }

            // stop letter / platform (arrival)
            if (! lastStop.value("stopLetter").toString().isEmpty()) {
                resultItem->setArrivalInfo(QString("🚏 %1").arg(lastStop.value("stopLetter").toString()));
            }

            else if (! lastStop.value("platformName").toString().isEmpty()) {
                resultItem->setArrivalInfo(QString("Pl. %1").arg(lastStop.value("platformName").toString()));
            }
        }

        // get the transport options (e.g. bus or tube lines)
        QVariantList stations = leg.value("routeOptions").toList();
        QStringList routeOptionsTrains;
        QStringList routeOptionsDirections;
        QString detailedInfo;

        QVariantList::const_iterator it_routeOpt;
        for (it_routeOpt = stations.constBegin(); it_routeOpt != stations.constEnd(); ++it_routeOpt) {

            QStringList routeOptionsDirectionsCurrentTrain;
            QVariantMap routeOption = it_routeOpt->toMap();

            QString currentRouteOption = routeOption.value("name").toString().toHtmlEscaped();
            routeOptionsTrains.push_back(currentRouteOption);
            detailedInfo += currentRouteOption + " to ";

            QVariantList directions = routeOption["directions"].toList();

             QVariantList::const_iterator it_directions;

             for (it_directions = directions.constBegin(); it_directions != directions.constEnd(); ++it_directions)
             {
                 QString currentDestination = it_directions->toString().replace("Underground Station", "").toHtmlEscaped();

                 if (! routeOptionsDirections.contains(currentDestination))
                 {
                    routeOptionsDirections.push_back(currentDestination);
                 }

                 routeOptionsDirectionsCurrentTrain.push_back(currentDestination);
             }

             detailedInfo += routeOptionsDirectionsCurrentTrain.join(" or ") + ".<br>";
        }

        //resultItem->setTrain(routeOptionsTrains.join(","));
        resultItem->setDirection(routeOptionsDirections.join(" or "));

        // walking
        if (mode == "walking")
        {
            const QString duration = leg.value("duration").toString();
            resultItem->setTrain(tr("Walk for %2 min").arg(duration));
        }

        // only one train --> put it in title
        else if (routeOptionsTrains.count() == 1)
        {
            resultItem->setTrain(routeOptionsTrains[0]);
        }
        else
        {
            resultItem->setTrain(mode);
            resultItem->setInfo(detailedInfo);
        }

        //resultItem->setDirection(leg.value("destination").toString());

        QStringList attributes;
        const QVariantList attrs = leg.value("attributes").toList();
        foreach (const QVariant &attr, attrs) {
            attributes << attr.toMap().value("title").toString();
        }
        //resultItem->setInfo(attributes.join(tr(", ")));

        result->appendItem(resultItem);
    }

    result->setDepartureStation(result->getItem(0)->departureStation());
    result->setArrivalStation(result->getItem(result->itemcount() - 1)->arrivalStation());

    cachedResults.insert(id, result);
}

QStringList ParserLondonTfl::getModesFromTrainRestrictions(int trainRestrictions)
{

    QStringList availableModes;

    switch(trainRestrictions){
    case all:
        availableModes.append(ModeBus);
        availableModes.append(ModeCableCar);
        availableModes.append(ModeCoach);
        availableModes.append(ModeDlr);
        availableModes.append(ModeRail);
        availableModes.append(ModeOverground);
        availableModes.append(ModeReplacementBus);
        availableModes.append(ModeRiverBus);
        availableModes.append(ModeRiverTour);
        availableModes.append(ModeTflRail);
        availableModes.append(ModeTram);
        availableModes.append(ModeTube);
        break;
    case bus_tram_tube_overground_dlr:
        availableModes.append(ModeBus);
        availableModes.append(ModeTram);
        availableModes.append(ModeTube);
        availableModes.append(ModeOverground);
        availableModes.append(ModeDlr);
        break;
    case rail_tube_overground_dlr:
        availableModes.append(ModeRail);
        availableModes.append(ModeTube);
        availableModes.append(ModeOverground);
        availableModes.append(ModeDlr);
        break;
    case tube_overground_dlr:
        availableModes.append(ModeTube);
        availableModes.append(ModeOverground);
        availableModes.append(ModeDlr);
        break;
    case bus_tram:
        availableModes.append(ModeBus);
        availableModes.append(ModeTram);
        break;
    case bus:
        availableModes.append(ModeBus);
        break;
    case rail:
        availableModes.append(ModeRail);
        break;
    case tube:
         availableModes.append(ModeTube);
        break;
    case overground:
        availableModes.append(ModeOverground);
        break;
    case dlr:
        availableModes.append(ModeDlr);
        break;
    }

   return availableModes;
}

// check the mode
bool ParserLondonTfl::doesModeMatchTrainRestrictions(const QString & mode, int trainRestrictions)
{
    return getModesFromTrainRestrictions(trainRestrictions).contains(mode);
}

// this is for reducing the network replies, ids not suitable for trainRestrictions are skipped
QStringList ParserLondonTfl::filterStopIdsByTrainRestrictions(const QStringList & stopIds, int trainRestrictions)
{
    QStringList output;

    QStringList::const_iterator j;
    for (j = stopIds.constBegin(); j != stopIds.constEnd(); ++j)
    {
        QString currentId = *j;

        bool includeInFilteredOutput = true;

        bool isTubeId = (currentId.left(8) == "940GZZLU");
        bool isDlrId = (currentId.left(8) == "940GZZDL");
        bool isRailOrOverGroundId = (! isTubeId && ! isDlrId && (currentId.left(4) == "940G"));
        bool isBusOrTramId = (!isTubeId && ! isDlrId && ! isRailOrOverGroundId);

        switch(trainRestrictions){
        case all:
        case bus_tram_tube_overground_dlr:
            break;
        case rail_tube_overground_dlr:
        case tube_overground_dlr:
            includeInFilteredOutput = (! isBusOrTramId);
            break;
        case bus_tram:
        case bus:
            includeInFilteredOutput = isBusOrTramId;
            break;
        case rail:
            includeInFilteredOutput = isRailOrOverGroundId;
            break;
        case tube:
            includeInFilteredOutput = isTubeId;
            break;
        case overground:
            includeInFilteredOutput = isRailOrOverGroundId;
            break;
        case dlr:
            includeInFilteredOutput = isDlrId;
            break;
        }

        if (includeInFilteredOutput)
        {
            output.push_back(currentId);
        }
    }

    return output;
}

void ParserLondonTfl::addTimeTableEntriesOfStopPoint(const QString & stopPointId, TimetableEntriesList & entriesList)
{
    QNetworkRequest request;
    QUrl relativeUrl (QString("/Stoppoint/%1/Arrivals").arg(stopPointId));

    request.setUrl(BaseUrl.resolved(relativeUrl));

    QNetworkReply *networkReply = NetworkManagerTimeTableSubQuery->get(request);

    QEventLoop loop;
    connect(networkReply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    QByteArray allData = networkReply->readAll();
    // qDebug() << "REPLY:>>>>>>>>>>>>\n" << allData;

    QVariantMap doc = parseJson("{ \"output\":" + allData + "}");

    if (doc.isEmpty()) {
        emit errorOccured(tr("Cannot parse reply from the server"));
        return;
    }

    QVariantList arrivals = doc.value("output").toList();

    QVariantList::const_iterator i;

    for (i = arrivals.constBegin(); i != arrivals.constEnd(); ++i) {

        QVariantMap arrival = i->toMap();

        // if the mode is not correct, we skip the value
        if (! doesModeMatchTrainRestrictions(arrival.value("modeName").toString(), timetableRestrictions) )
        {
            continue;
        }

        TimetableEntry entry;

        entry.currentStation = arrival.value("stationName").toString();
        entry.destinationStation = arrival.value("destinationName").toString();
        entry.time = QDateTime::fromString(arrival.value("expectedArrival").toString(), "yyyy-MM-ddTHH:mm:ssZ").time();
        entry.platform = arrival.value("platformName").toString();
        entry.trainType = shortLineName(arrival.value("lineName").toString());
        entriesList.append(entry);
    }
}
