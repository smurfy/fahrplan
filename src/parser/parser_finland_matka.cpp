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

#include <QUrl>
#ifdef BUILD_FOR_QT5
#include <QUrlQuery>
#include <QTimeZone>
#endif
#include <QLocale>
#include <QRegExp>
#include <QVariantMap>
#include <QFile>

#include "parser_finland_matka.h"

#define MULTILINE(...) #__VA_ARGS__

ParserFinlandMatka::ParserFinlandMatka(QObject *parent) :
        ParserAbstract(parent)
{
}

bool ParserFinlandMatka::supportsGps()
{
    return true;
}

bool ParserFinlandMatka::supportsVia()
{
    return true;
}

bool ParserFinlandMatka::supportsTimeTable()
{
    return true;
}

bool ParserFinlandMatka::supportsTimeTableDirection()
{
    return true;
}

QStringList ParserFinlandMatka::getTrainRestrictions()
{
    QStringList list;
    // Note: When changing the list below, also update the enum TransportModeFilter
    list << tr("All")
         << tr("Bus")
         << tr("Train")
         << tr("Airplane")
         << tr("Bus and train")
         << tr("Bus and airplane")
         << tr("Train and airplane");
    return list;
}

QStringList ParserFinlandMatka::selectedTransportModes(int selection)
{
    QStringList modes;
    if (selection == UseAll)
        modes << "TRANSIT"; // TRANSIT == some kind of public means of transportation
    if (selection == UseBus || selection == UseBusAndTrain || selection == UseBusAndAirplane)
        modes << "BUS";
    if (selection == UseTrain || selection == UseBusAndTrain || selection == UseTrainAndAirplane)
        modes << "RAIL";
    if (selection == UseAirplane || selection == UseTrainAndAirplane || selection == UseBusAndAirplane)
        modes << "AIRPLANE";
    modes << "WALK" << "FERRY" << "SUBWAY" << "TRAM"; // These should always be allowed
    return modes;
}

void ParserFinlandMatka::findStationsByName(const QString &stationName)
{
    if (stationName.length() < 2)
        return;
    if (currentRequestState != FahrplanNS::noneRequest)
        return;
    currentRequestState = FahrplanNS::stationsByNameRequest;

    QUrl url("https://api.digitransit.fi/geocoding/v1/search");
#if defined(BUILD_FOR_QT5)
    QUrlQuery query;
#else
    QUrl query;
#endif
    query.addQueryItem("text", stationName); // Automatically percent encoded
    query.addQueryItem("lang", languageCode());
    query.addQueryItem("size", "50"); // Number of wanted results
    // Don't include "venue", since that will give a lot of noise when searching
    query.addQueryItem("layers", "stop,station,neighbourhood,locality,localadmin,county,region");
#if defined(BUILD_FOR_QT5)
    url.setQuery(query);
#else
    url.setQueryItems(query.queryItems());
#endif

    sendRequest(url);
}

void ParserFinlandMatka::findStationsByCoordinates(qreal longitude, qreal latitude)
{
    if (currentRequestState != FahrplanNS::noneRequest)
        return;
    currentRequestState = FahrplanNS::stationsByCoordinatesRequest;

    QUrl url("https://api.digitransit.fi/geocoding/v1/reverse");
#if defined(BUILD_FOR_QT5)
    QUrlQuery query;
#else
    QUrl query;
#endif
    query.addQueryItem("point.lat", QString::number(latitude));
    query.addQueryItem("point.lon", QString::number(longitude));
    query.addQueryItem("size", "50"); // Number of wanted results
    // Don't include "venue", since that will give a lot of noise when searching
    query.addQueryItem("layers", "stop,station,neighbourhood,locality,localadmin,county,region");
    query.addQueryItem("lang", languageCode());
#if defined(BUILD_FOR_QT5)
    url.setQuery(query);
#else
    url.setQueryItems(query.queryItems());
#endif
    sendRequest(url);
}

void ParserFinlandMatka::parseStationsByName(QNetworkReply *networkReply)
{
    QByteArray allData(networkReply->readAll());
    if (networkReply->rawHeader("Content-Encoding") == "gzip") {
        allData = gzipDecompress(allData);
    }
//    qDebug() << "Reply:\n" << allData;

    QVariantMap doc = parseJson(allData);
    if (doc.isEmpty()) {
        emit errorOccured(tr("Cannot parse reply from the server"));
        return;
    }
    if (doc.value("geocoding").toMap().contains("errors")) {
        QStringList errorMessages;
        Q_FOREACH (const QVariant& error, doc.value("geocoding").toMap().value("errors").toList()) {
            errorMessages.append(error.toString());
        }
        emit errorOccured(tr("Server replied") + ":\n\n" + errorMessages.join("\n\n"));
        return;
    }

    QVariantList features = doc.value("features").toList();
    StationsList results;
    Q_FOREACH (const QVariant& featureData, features) {
        const QVariantMap& feature(featureData.toMap());
        const QVariantList& coordinates(feature.value("geometry").toMap().value("coordinates").toList());
        if (coordinates.size() != 2) {
            qDebug() << "Invalid coordinates:" << feature;
            continue;
        }
        const QVariantMap& properties(feature.value("properties").toMap());
        Station s;
        s.id = properties.value("id");
        s.name = properties.value("label").toString();
        s.type = properties.value("layer").toString();

        // Ignore stations and stops from any other provider than GTFS
        // They just mess up when searching for timetables
        if ((s.type == "stop" || s.type == "station") &&
                properties.value("source").toString() != "gtfs")
            continue;

        s.longitude = coordinates.at(0).toDouble();
        s.latitude = coordinates.at(1).toDouble();
        if (s.id.toString().startsWith("GTFS:MATKA:30281_")) // Looks like an airport
            s.miscInfo = tr("Airport");
        else if (s.id.toString().startsWith("GTFS:MATKA:4_")) // Looks like a railway station
            s.miscInfo = tr("Railway station");
        else
            s.miscInfo = prettyPlaceType(s.type);
        results.append(s);

        qDebug() << s.miscInfo << s.id.toString() << s.name << s.latitude << s.longitude;
    }
    qDebug() << "Found" << results.size() << "results";
    emit stationsResult(results);
}

void ParserFinlandMatka::parseStationsByCoordinates(QNetworkReply *networkReply)
{
    parseStationsByName(networkReply);
}

void ParserFinlandMatka::getTimeTableForStation(const Station &currentStation,
                                            const Station &directionStation,
                                            const QDateTime &dateTime,
                                            Mode mode,
                                            int trainrestrictions)
{
    if (currentStation.type != "stop" && currentStation.type != "station") {
        emit errorOccured(tr("Timetables are only available for stops and stations."));
        return;
    }
    if (currentRequestState != FahrplanNS::noneRequest)
        return;
    currentRequestState = FahrplanNS::getTimeTableForStationRequest;

    lastTimetableSearch.currentStation = currentStation;
    lastTimetableSearch.directionStation = directionStation;
    lastTimetableSearch.dateTime = dateTime;
    lastTimetableSearch.mode = mode;
    lastTimetableSearch.restrictions = trainrestrictions;

    QString query(MULTILINE(
        query ($station: String!, $startTime: Long, $timeRange: Int, $numberOfDepartures: Int) {
            %1(id: $station) {
                stopTimes: stoptimesWithoutPatterns(startTime: $startTime,
                        timeRange: $timeRange, numberOfDepartures: $numberOfDepartures) {
                    realtime
                    scheduledArrival
                    realtimeArrival
                    scheduledDeparture,
                    realtimeDeparture
                    stopHeadsign
                    serviceDay
                    stop {
                        gtfsId
                        name
                        lat
                        lon
                        code
                        platformCode
                        desc
                    }
                    trip {
                        pattern {
                            id
                            directionId
                            headsign
                            route {
                                mode
                                shortName
                                longName
                                agency {
                                    name
                                    url
                                }
                                patterns {
                                    id
                                    headsign
                                }
                            }
                            stops {
                                gtfsId
                                name
                          }
                        }
                    }
                }
            }
        }
    ));
    query.replace("%1", currentStation.type);

    QVariantMap request;
    request["query"] = query;
    QVariantMap variables;
    variables["station"] = timetableStationID(currentStation.id.toString(), mode);
#ifdef BUILD_FOR_QT5
    QDateTime helsinkiTime(dateTime.toTimeZone(QTimeZone("Europe/Helsinki")));
    variables["startTime"] = QString::number(helsinkiTime.toMSecsSinceEpoch() / 1000);
#else
    variables["startTime"] = QString::number(dateTime.toMSecsSinceEpoch() / 1000);
#endif
    variables["timeRange"] = 86400; // Search for arrivals/departures the next 24 hours
    variables["numberOfDepartures"] = 50;
    request["variables"] = variables;
    sendRequest(QUrl("https://api.digitransit.fi/routing/v1/routers/finland/index/graphql"), request);
}

void ParserFinlandMatka::parseTimeTable(QNetworkReply *networkReply)
{
    TimetableEntriesList timetable;
    QByteArray allData(networkReply->readAll());
    if (networkReply->rawHeader("Content-Encoding") == "gzip") {
        allData = gzipDecompress(allData);
    }
//    qDebug() << "Reply:\n" << allData;

    QVariantMap doc = parseJson(allData);
    if (doc.isEmpty()) {
        emit errorOccured(tr("Cannot parse reply from the server"));
        return;
    }
    if (doc.contains("errors")) {
        QStringList errorMessages;
        Q_FOREACH (const QVariant& error, doc.value("errors").toList()) {
            errorMessages.append(error.toMap().value("message").toString());
        }
        emit errorOccured(tr("Server replied") + ":\n\n" + errorMessages.join("\n\n"));
        return;
    }
    QMap<QDateTime, QString> timesAndPatternIDs;
    QVariantMap stationOrStop(doc.value("data").toMap().value(lastTimetableSearch.currentStation.type).toMap());
    const QVariantList& stopTimes(stationOrStop.value("stopTimes").toList());
    Q_FOREACH (const QVariant& stopTimeData, stopTimes) {
        const QVariantMap& stopTime(stopTimeData.toMap());
        const QVariantMap& stop(stopTime.value("stop").toMap());
        const QVariantMap& pattern(stopTime.value("trip").toMap().value("pattern").toMap());
        const QVariantList& patternStops(pattern.value("stops").toList());
        const QString& patternID(pattern.value("id").toString());
        const QVariantMap& route(pattern.value("route").toMap());
        const QString& stopName(stop.value("name").toString());
        QString transportMode(route.value("mode").toString());
        TimetableEntry entry;
        QStringList info;

        entry.currentStation = stopName;
        QString platformCode(stop.value("platformCode").toString());
        if (!platformCode.isEmpty())
            entry.platform = platformCode;
        QString stopCode(stop.value("code").toString());
        if (!stopCode.isEmpty()) {
            if (entry.platform.isEmpty())
                entry.platform = stopCode;
            else if (!stopCode.isEmpty())
                entry.platform += QString(" [%1]").arg(stopCode);
        }
        QString stopDesc(stop.value("desc").toString());
        if (!stopDesc.isEmpty() && stopDesc != stopName) {
            if (entry.platform.isEmpty())
                entry.platform = stopDesc;
            else
                entry.platform += QString(" (%1)").arg(stopDesc);
        }
        entry.latitude = stop.value("lat").toDouble();
        entry.longitude = stop.value("lon").toDouble();
        entry.trainType = transportModeName(transportMode);
        QString routeName(route.value("shortName").toString());
        if (!routeName.isEmpty())
            entry.trainType += " " + routeName;
        qDebug() << "Found departure with" << entry.trainType;

        // Filter on transport type
        if (!transportModeIsSelected(transportMode, lastTimetableSearch.restrictions)) {
            qDebug() << "Transport mode not selected";
            continue;
        }

        // Info that depends on whether we search for departures or arrivals
        int relativeTime;
        if (lastTimetableSearch.mode == Arrival) {
            if (!patternStops.isEmpty() && patternStops.first().toMap().value("gtfsId").toString() ==
                    stop.value("gtfsId")) {
                qDebug() << "This is the first stop - skipping arrival from self";
                continue;
            }
            // "origin station" would be more correct for "destination station" in this case.
            // In contrast to when searching for departures, there is no "reverse direction
            // headsign" to use, so look at the patterns for this route and if there are exactly
            // two patterns, the arrival pattern (reverse direction) should be the one with an
            // ID different from the departure pattern.
            QVariantList routePatterns(route.value("patterns").toList());
            if (routePatterns.size() == 2) {
                Q_FOREACH (const QVariant& routePatternData, routePatterns) {
                    const QVariantMap& routePattern(routePatternData.toMap());
                    if (routePattern.value("id").toString() != patternID) {
                        entry.destinationStation = routePattern.value("headsign").toString();
                    }
                }
            }
            if (entry.destinationStation.isEmpty()) {
                // Fall back to the name of the first stop
                if (patternStops.isEmpty())
                    continue;
                entry.destinationStation = patternStops.first().toMap().value("name").toString();
            }
            if (stopTime.value("realtime").toBool()) {
                relativeTime = stopTime.value("realtimeArrival").toInt();
                info << tr("Realtime info");
            } else {
                relativeTime = stopTime.value("scheduledArrival").toInt();
            }
        } else {
            if (!patternStops.isEmpty() && patternStops.last().toMap().value("gtfsId").toString() ==
                    stop.value("gtfsId")) {
                qDebug() << "This is the last stop - skipping departure to self";
                continue;
            }
            entry.destinationStation = stopTime.value("stopHeadsign").toString();
            if (entry.destinationStation.isEmpty())
                entry.destinationStation = pattern.value("headsign").toString();
            if (stopTime.value("realtime").toBool()) {
                relativeTime = stopTime.value("realtimeDeparture").toInt();
                info << tr("Realtime info");
            } else {
                relativeTime = stopTime.value("scheduledDeparture").toInt();
            }
        }
        QDateTime dateTime;
#ifdef BUILD_FOR_QT5
        dateTime.setTimeZone(QTimeZone("Europe/Helsinki"));
#endif
        qlonglong baseTime = stopTime.value("serviceDay").toLongLong();
        dateTime.setMSecsSinceEpoch((baseTime + relativeTime) * 1000);
        entry.time = dateTime.toLocalTime().time();

        if (timesAndPatternIDs.values(dateTime).contains(entry.trainType)) {
            // The service should not give duplicates but sometimes does anyway
            // (at least for airports). The different entries can even have
            // different route and pattern ID's.
            qDebug() << "Skipping duplicate entry: " << entry.trainType << "at"
                     << dateTime.toLocalTime().toString() << "already exists";
            continue;
        }
        timesAndPatternIDs.insertMulti(dateTime, entry.trainType);

        // Filter on direction station
        if (lastTimetableSearch.directionStation.valid) {
            // It can be tricky to pick exactly the correct stop as a direction station, since many
            // stops and POI's etc have the same or similar names, so be a bit more generous.
            // First check if the name of the direction station matches the destination/origin
            // station name (use the pattern headsign for this). This is not necessarily the same
            // as the stop headsign, which sometimes has "via" info added. If we search for
            // departures use the pattern headsign. If we search for arrivals, we have used the
            // pattern headsign for the "destination" (origin) station, so use that.
            QString directionStationName(lastTimetableSearch.directionStation.name.split(",").first());
            bool foundNameMatch = false;
            if (lastTimetableSearch.mode == Arrival) {
                if (entry.destinationStation.compare(directionStationName, Qt::CaseInsensitive) == 0) {
                    qDebug() << "Origin station" << entry.destinationStation << "matches direction";
                    foundNameMatch = true;
                } else {
                    qDebug() << "Origin station" << entry.destinationStation << "does not match direction"
                             << directionStationName;
                }
            } else {
                if (pattern.value("headsign").toString().compare(directionStationName, Qt::CaseInsensitive) == 0) {
                    qDebug() << "Destination station" << entry.destinationStation << "matches direction";
                    foundNameMatch = true;
                } else {
                    qDebug() << "Destination station" << entry.destinationStation << "does not match direction"
                             << directionStationName;
                }
            }
            if (!foundNameMatch) {
                // No name destination/origin station name match - check ID's of intermediate stops
                const QString& currentStopID(stop.value("gtfsId").toString());
                QString directionStopID(timetableStationID(lastTimetableSearch.directionStation.id.toString(),
                                                           lastTimetableSearch.mode));
                int indexOfCurrentStop = -1;
                int indexOfDirectionStop = -1;
                for (int i = 0; i < patternStops.size(); ++i) {
                    QString stopID(patternStops.at(i).toMap().value("gtfsId").toString());
                    if (stopID == currentStopID) {
                        indexOfCurrentStop = i;
                        if (indexOfDirectionStop > -1)
                            break;
                    } else if (stopID == directionStopID) {
                        indexOfDirectionStop = i;
                        if (indexOfCurrentStop > -1)
                            break;
                    }
                }
                if (indexOfCurrentStop < 0 || indexOfDirectionStop < 0) {
                    qDebug() << "Current stop or direction stop not found in list of stops for this pattern";
                    continue;
                }
                if (lastTimetableSearch.mode == Arrival) {
                    if (indexOfDirectionStop > indexOfCurrentStop) {
                        qDebug() << "No arrivals from" << lastTimetableSearch.directionStation.name <<
                                    "for this pattern (only departures)";
                        continue;
                    }
                } else {
                    if (indexOfDirectionStop < indexOfCurrentStop) {
                        qDebug() << "No departures to" << lastTimetableSearch.directionStation.name <<
                                    "for this pattern (only arrivals)";
                        continue;
                    }
                }
            }
        }

        // Agency info
        const QVariantMap& agency(route.value("agency").toMap());
        QString agencyName = agency.value("name").toString();
        QString agencyURL = agency.value("url").toString();
        if (!agencyName.isEmpty()) {
            if (agencyURL.isEmpty())
                info << agencyName;
            else
                info << "<a href=\"" + agencyURL + "\">" + agencyName + "</a>";
        }

        if (!info.isEmpty())
            entry.miscInfo = info.join("<br/>");

        timetable.append(entry);
    }

    emit timetableResult(timetable);
}

void ParserFinlandMatka::searchJourney(const Station &departureStation, const Station &viaStation,
                                   const Station &arrivalStation, const QDateTime &dateTime,
                                   Mode mode, int trainRestrictions)
{
    if (currentRequestState != FahrplanNS::noneRequest)
        return;
    currentRequestState = FahrplanNS::searchJourneyRequest;
    numberOfUnsuccessfulEarlierSearches = 0;
    numberOfUnsuccessfulLaterSearches = 0;
    internalSearchJourney(departureStation, viaStation, arrivalStation, dateTime, mode, trainRestrictions);
}

void ParserFinlandMatka::sendRequest(QUrl url, QVariantMap request)
{
    QList<QPair<QByteArray,QByteArray> > additionalHeaders;
    additionalHeaders.append(QPair<QByteArray,QByteArray>("Content-Type", "application/json"));
    additionalHeaders.append(QPair<QByteArray,QByteArray>("Accept-Encoding", "gzip"));
    qDebug() << "Sending request to " << url.toString();
    if (request.isEmpty()) {
        sendHttpRequest(url, NULL, additionalHeaders);
    } else {
        qDebug() << serializeToJson(request);
        sendHttpRequest(url, serializeToJson(request), additionalHeaders);
    }
}

void ParserFinlandMatka::internalSearchJourney(const Station &departureStation, const Station &viaStation,
                                               const Station &arrivalStation, const QDateTime &dateTime,
                                               Mode mode, int trainRestrictions)
{
    lastJourneySearch.dateTime = dateTime;
    lastJourneySearch.from = departureStation;
    lastJourneySearch.to = arrivalStation;
    lastJourneySearch.via = viaStation;
    lastJourneySearch.restrictions = trainRestrictions;
    lastJourneySearch.mode = mode;
    lastJourneySearch.resultCount = 0;
    lastJourneySearch.restrictionStrings = selectedTransportModes(trainRestrictions);

    // The routing optimization values below were copied from beta.matka.fi
    QString query(MULTILINE(
        query ($date: String, $time: String, $fromPlace: String, $toPlace: String, $locale: String,
               $intermediatePlaces: [InputCoordinates])
        {
            plan(
                date: $date
                time: $time
                fromPlace: $fromPlace
                toPlace: $toPlace
                numItineraries: 5
                locale: $locale
                intermediatePlaces: $intermediatePlaces
                walkReluctance: 2
                walkBoardCost: 600
                minTransferTime: 180
                walkSpeed: 1.2
                maxWalkDistance: 10000
                wheelchair: false
                disableRemainingWeightHeuristic: false
            ) {
                date
                itineraries {
                    startTime
                    endTime
                    duration
                    walkDistance
                    legs {
                        mode
                        distance
                        duration
                        realTime
                        startTime
                        trip {
                            tripHeadsign
                        }
                        route {
                            agency {
                                name
                                url
                            }
                            shortName
                        }
                        from {
                            name
                            stop {
                                name
                                code
                                desc
                            }
                        }
                        endTime
                        to {
                            name
                            stop {
                                name
                                code
                                desc
                            }
                        }
                    }
                }
            }
        }
    ));
    QVariantMap request;
    request["query"] = query;

    // Compile a list of via stops
    QVariantList viaList;
    if (viaStation.valid) {
        QVariantMap coordinates;
        coordinates["lat"] = viaStation.latitude;
        coordinates["lon"] = viaStation.longitude;
        viaList.append(coordinates);
    }

    QVariantMap variables;
#ifdef BUILD_FOR_QT5
    QDateTime helsinkiTime(dateTime.toTimeZone(QTimeZone("Europe/Helsinki")));
    variables["date"] = helsinkiTime.date().toString("yyyy-MM-dd");
    variables["time"] = helsinkiTime.time().toString("hh:mm");
#else
    variables["date"] = dateTime.date().toString("yyyy-MM-dd");
    variables["time"] = dateTime.time().toString("hh:mm");
#endif

    // Not exactly sure that the name is needed in fromPlace and toPlace,
    // but that's the format used on beta.matka.fi
    variables["fromPlace"] = QString("%1::%2,%3").arg(departureStation.name)
            .arg(departureStation.latitude).arg(departureStation.longitude);
    variables["toPlace"] = QString("%1::%2,%3").arg(arrivalStation.name)
            .arg(arrivalStation.latitude).arg(arrivalStation.longitude);

    variables["locale"] = languageCode();
    variables["arriveBy"] = (mode == Arrival);
    variables["modes"] = lastJourneySearch.restrictionStrings.join(",");
    variables["intermediatePlaces"] = viaList;
    request["variables"] = variables;
    sendRequest(QUrl("https://api.digitransit.fi/routing/v1/routers/finland/index/graphql"), request);
}

void ParserFinlandMatka::parseSearchJourney(QNetworkReply *networkReply)
{
    JourneyResultList *journeyList = new JourneyResultList();
    QByteArray allData(networkReply->readAll());
    if (networkReply->rawHeader("Content-Encoding") == "gzip") {
        allData = gzipDecompress(allData);
    }
//    qDebug() << "Reply:\n" << allData;

    QVariantMap doc = parseJson(allData);
    if (doc.isEmpty()) {
        emit errorOccured(tr("Cannot parse reply from the server"));
        return;
    }
    if (doc.contains("errors")) {
        QStringList errorMessages;
        Q_FOREACH (const QVariant& error, doc.value("errors").toList()) {
            errorMessages.append(error.toMap().value("message").toString());
        }
        emit errorOccured(tr("Server replied") + ":\n\n" + errorMessages.join("\n\n"));
        return;
    }
    const QVariantList& itineraries(doc.value("data").toMap().value("plan").toMap().value("itineraries").toList());
    int journeyCounter = 0;
    bool hasFoundWalkOnlyRoute = false;
    Q_FOREACH (const QVariant itineraryVar, itineraries) {
        const QVariantMap& itinerary(itineraryVar.toMap());
        QString journeyID = QString::number(journeyCounter);
        QList<JourneyDetailResultItem*> segments = parseJourneySegments(itinerary);
        bool walkDistanceOK;
        double walkDistance = itinerary.value("walkDistance").toDouble(&walkDistanceOK);

        if (segments.isEmpty())
            continue;

        // Duration
        bool durationOK;
        int minutes = itinerary.value("duration").toDouble(&durationOK) / 60;
        int hours = minutes / 60;
        minutes = minutes % 60;
        QString duration = QString("%1:%2").arg(hours).arg(minutes, 2, 10, QChar('0'));

        // Compile list of transport modes used
        QStringList transportModes;
        foreach (JourneyDetailResultItem* segment, segments) {
            if (segment->internalData1() != "WALK")
                transportModes.append(segment->train());
        }
        // When the distance is short, an option with only "walk" can be present
        if (transportModes.count() == 0 && segments.count() == 1) {
            if (hasFoundWalkOnlyRoute) {
                // For some reason there sometimes are several "walk only" routes - only use the first one
                qDebug() << "Skipping walk only route (have already got one)";
                continue;
            } else {
                transportModes.append(segments.first()->train());
                hasFoundWalkOnlyRoute = true;
            }
        }

        JourneyDetailResultList* journeyDetails = new JourneyDetailResultList;
        foreach (JourneyDetailResultItem* segment, segments)
            journeyDetails->appendItem(segment);
        journeyDetails->setId(journeyID);
        journeyDetails->setDepartureStation(segments.first()->departureStation());
        journeyDetails->setDepartureDateTime(segments.first()->departureDateTime());
        journeyDetails->setArrivalStation(segments.last()->arrivalStation());
        journeyDetails->setArrivalDateTime(segments.last()->arrivalDateTime());
        if (durationOK)
            journeyDetails->setDuration(duration);
        cachedResults.insert(journeyID, journeyDetails);

        // Indicate in the departure/arrival times if they are another day (e.g. "14:37+1")
        int depDayDiff = lastJourneySearch.dateTime.date().daysTo(journeyDetails->departureDateTime().date());
        QString depTime = journeyDetails->departureDateTime().toString("HH:mm");
        if (depDayDiff > 0)
            depTime += "+" + QString::number(depDayDiff);
        else if (depDayDiff < 0)
            depTime += QString::number(depDayDiff);
        int arrDayDiff = lastJourneySearch.dateTime.date().daysTo(journeyDetails->arrivalDateTime().date());
        QString arrTime = journeyDetails->arrivalDateTime().toString("HH:mm");
        if (arrDayDiff > 0)
            arrTime += "+" + QString::number(arrDayDiff);
        else if (arrDayDiff < 0)
            arrTime += QString::number(arrDayDiff);

        JourneyResultItem* journey = new JourneyResultItem;
        journey->setId(journeyID);
        journey->setDate(segments.first()->departureDateTime().date());
        journey->setDepartureTime(depTime);
        journey->setArrivalTime(arrTime);
        journey->setTrainType(transportModes.join(", "));
        journey->setDuration(journeyDetails->duration());
        journey->setTransfers(QString::number(transportModes.count()-1));
        if (walkDistanceOK && walkDistance > 0.0)
            journey->setMiscInfo(tr("Walk %1").arg(formatDistance(walkDistance)));
        journeyList->appendItem(journey);

        if (journeyCounter == 0) {
            if (lastJourneySearch.mode == Departure)
                lastJourneySearch.firstOption = journeyDetails->departureDateTime();
            else
                lastJourneySearch.firstOption = journeyDetails->arrivalDateTime();
        }
        if (lastJourneySearch.mode == Departure)
            lastJourneySearch.lastOption = journeyDetails->departureDateTime();
        else
            lastJourneySearch.lastOption = journeyDetails->arrivalDateTime();

        ++journeyCounter;
    }
    lastJourneySearch.resultCount = journeyCounter;
    journeyList->setDepartureStation(lastJourneySearch.from.name);
    journeyList->setArrivalStation(lastJourneySearch.to.name);
    QString modeString;
    if (lastJourneySearch.mode == Arrival)
        modeString = tr("Arrivals");
    else
        modeString = tr("Departures");
    journeyList->setTimeInfo(modeString + " " + lastJourneySearch.dateTime.toString(tr("ddd MMM d, HH:mm")));

    emit journeyResult(journeyList);
}

QList<JourneyDetailResultItem*> ParserFinlandMatka::parseJourneySegments(const QVariantMap& itinerary)
{
    QList<JourneyDetailResultItem*> results;

    Q_FOREACH (const QVariant& legVar, itinerary.value("legs").toList()) {
        const QVariantMap& leg(legVar.toMap());
        qDebug() << "Parsing journey segment";
        const QVariantMap from(leg.value("from").toMap());
        const QVariantMap to(leg.value("to").toMap());
        if (from.isEmpty() || to.isEmpty())
            continue;
        JourneyDetailResultItem* journeySegment = new JourneyDetailResultItem();
        QString transportMode = leg.value("mode").toString();
        qDebug() << "Transport mode:" << transportMode;
        if (!lastJourneySearch.restrictionStrings.contains("TRANSIT") &&
                !lastJourneySearch.restrictionStrings.contains(transportMode)) {
            qDebug() << "This journey segment is using an unselected transport mode - skipping journey";
            Q_FOREACH (JourneyDetailResultItem* item, results) {
                delete item;
                item = NULL;
            }
            return QList<JourneyDetailResultItem*>();
        }
        QStringList info;
        bool distanceOK;
        double distance = leg.value("distance").toDouble(&distanceOK);
        if (distanceOK)
            info.append(formatDistance(distance));
        journeySegment->setDepartureStation(parseNodeName(from));
        journeySegment->setArrivalStation(parseNodeName(to));
        QDateTime depDt;
#ifdef BUILD_FOR_QT5
        depDt.setTimeZone(QTimeZone("Europe/Helsinki"));
#endif
        depDt.setMSecsSinceEpoch(leg.value("startTime").toLongLong());
        qDebug() << "departing" << journeySegment->departureStation() << depDt;
        journeySegment->setDepartureDateTime(depDt.toLocalTime());
        QDateTime arrDt;
#ifdef BUILD_FOR_QT5
        arrDt.setTimeZone(QTimeZone("Europe/Helsinki"));
#endif
        arrDt.setMSecsSinceEpoch(leg.value("endTime").toLongLong());
        journeySegment->setArrivalDateTime(arrDt.toLocalTime());

        if (transportMode == "WALK") {
            journeySegment->setTrain(tr("Walk"));
            journeySegment->setInternalData1("WALK");
        } else {
            QString transportString(transportModeName(transportMode));
            const QVariantMap& route(leg.value("route").toMap());
            if (!route.isEmpty()) {
                QString routeName(route.value("shortName").toString());
                qDebug() << "Route short name:" << routeName;
                if (!routeName.isEmpty())
                    transportString += " " + routeName;
                const QVariantMap& agency(route.value("agency").toMap());
                if (!agency.isEmpty()) {
                    QString agencyName = agency.value("name").toString();
                    QString agencyURL = agency.value("url").toString();
                    if (!agencyName.isEmpty()) {
                        if (agencyURL.isEmpty())
                            info.append(agencyName);
                        else
                            info.append("<a href=\"" + agencyURL + "\">" + agencyName + "</a>");
                    }
                }
            }
            const QVariantMap& trip(leg.value("trip").toMap());
            if (!trip.isEmpty()) {
                QString tripHeadSign(trip.value("tripHeadsign").toString());
                if (!tripHeadSign.isEmpty())
                    journeySegment->setDirection(tripHeadSign);
            }
            if (leg.value("realTime").toBool())
                info.append(tr("Realtime info"));
            journeySegment->setTrain(transportString);
        }
        if (!info.isEmpty())
            journeySegment->setInfo(info.join("<br/>"));

        results.append(journeySegment);
    }
    return results;
}

void ParserFinlandMatka::getJourneyDetails(const QString &id)
{
    if (cachedResults.contains(id))
        emit journeyDetailsResult(cachedResults.value(id));
    else
        emit errorOccured(tr("No journey details found."));
}

void ParserFinlandMatka::searchJourneyLater()
{
    // If the last "later" search didn't give any new results, try searching one
    // hour later than last time, otherwise search on the time of the last result option.
    QDateTime time;
    if (lastJourneySearch.resultCount > 0 && lastJourneySearch.mode == Departure)
        time = lastJourneySearch.lastOption.addSecs(numberOfUnsuccessfulLaterSearches * 3600);
    else
        time = lastJourneySearch.dateTime.addSecs(3600);

    currentRequestState = FahrplanNS::searchJourneyLaterRequest;
    internalSearchJourney(lastJourneySearch.from, lastJourneySearch.via, lastJourneySearch.to,
                          time, lastJourneySearch.mode, lastJourneySearch.restrictions);
}

void ParserFinlandMatka::parseSearchLaterJourney(QNetworkReply *networkReply)
{
    QDateTime oldFirstOption = lastJourneySearch.firstOption;
    QDateTime oldLastOption = lastJourneySearch.lastOption;
    parseSearchJourney(networkReply);
    if (oldFirstOption != lastJourneySearch.firstOption)
        numberOfUnsuccessfulEarlierSearches = 0;
    if (oldLastOption != lastJourneySearch.lastOption)
        numberOfUnsuccessfulLaterSearches = 0;
    else
        ++numberOfUnsuccessfulLaterSearches;
}

void ParserFinlandMatka::searchJourneyEarlier()
{
    // If last the "earlier" search didn't give any new results, try searching one
    // hour earlier than last time, otherwise search on the time of the first result
    // option minus one hour.
    QDateTime time;
    if (lastJourneySearch.resultCount > 0 && lastJourneySearch.mode == Arrival)
        time = lastJourneySearch.firstOption.addSecs(numberOfUnsuccessfulEarlierSearches * -3600);
    else
        time = lastJourneySearch.dateTime.addSecs(-3600);
    currentRequestState = FahrplanNS::searchJourneyEarlierRequest;
    internalSearchJourney(lastJourneySearch.from, lastJourneySearch.via, lastJourneySearch.to,
                          time, lastJourneySearch.mode, lastJourneySearch.restrictions);
}

void ParserFinlandMatka::parseSearchEarlierJourney(QNetworkReply *networkReply)
{
    QDateTime oldFirstOption = lastJourneySearch.firstOption;
    QDateTime oldLastOption = lastJourneySearch.lastOption;
    parseSearchJourney(networkReply);
    if (oldFirstOption != lastJourneySearch.firstOption)
        numberOfUnsuccessfulEarlierSearches = 0;
    else
        ++numberOfUnsuccessfulEarlierSearches;
    if (oldLastOption != lastJourneySearch.lastOption)
        numberOfUnsuccessfulLaterSearches = 0;
}

void ParserFinlandMatka::parseJourneyDetails(QNetworkReply *networkReply)
{
    Q_UNUSED(networkReply)
    // Should never happen since we get all info about all journeys at once
    // and therefore never need to send any journeyDetailsRequest.
}

QString ParserFinlandMatka::formatDistance(double distance)
{
    if (distance > 10000)
        // E.g. 16 km
        return QString("%1 %2").arg(QString::number(distance/1000.0, 'f', 0)).arg("km");
    else if (distance > 1000)
        // E.g. 5.6 km
        return QString("%1 %2").arg(QString::number(distance/1000.0, 'f', 1)).arg("km");
    else
        // E.g. 391 m
        return QString("%1 %2").arg(QString::number(distance, 'f', 0)).arg("m");
}

QString ParserFinlandMatka::transportModeName(const QString& mode)
{
    if (mode == "AIRPLANE")
        return tr("Flight");
    else if (mode == "BUS")
        return tr("Bus");
    else if (mode == "CABLE_CAR")
        return tr("Cable car");
    else if (mode == "FERRY")
        return tr("Ferry");
    else if (mode == "GONDOLA")
        return tr("Gondola");
    else if (mode == "RAIL")
        return tr("Train");
    else if (mode == "SUBWAY")
        return tr("Metro");
    else if (mode == "TRAM")
        return tr("Tram");
    else if (mode == "WALK")
        return tr("Walk");
    else
        return tr("Unknown transport type: %1").arg(mode);
}

bool ParserFinlandMatka::transportModeIsSelected(const QString& mode, int selectedFilter)
{
    qDebug() << "Checking if" << transportModeName(mode) << "is selected...";
    bool busSelected = false;
    bool trainSelected = false;
    bool airplaneSelected = false;
    switch (selectedFilter) {
    case UseAll:
        busSelected = true;
        trainSelected = true;
        airplaneSelected = true;
        break;
    case UseBus:
        busSelected = true;
        break;
    case UseTrain:
        trainSelected = true;
        break;
    case UseAirplane:
        airplaneSelected = true;
        break;
    case UseBusAndTrain:
        busSelected = true;
        trainSelected = true;
        break;
    case UseBusAndAirplane:
        busSelected = true;
        airplaneSelected = true;
        break;
    case UseTrainAndAirplane:
        trainSelected = true;
        airplaneSelected = true;
        break;
    }

    if (mode == "AIRPLANE")
        return airplaneSelected;
    else if (mode == "BUS")
        return busSelected;
    else if (mode == "RAIL")
        return trainSelected;
    else // CABLE_CAR, SUBWAY, FERRY, GONDOLA, TRAM, WALK
        return true;
}

QString ParserFinlandMatka::parseNodeName(const QVariantMap& node)
{
    QString name(node.value("name").toString());
    if (!node.value("stop").isNull()) {
        QString stopCode(node.value("stop").toMap().value("code").toString());
        if (!stopCode.isEmpty())
            name += QString(" [%1]").arg(stopCode);
        QString stopDesc(node.value("stop").toMap().value("desc").toString());
        if (!stopDesc.isEmpty() && stopDesc != node.value("name").toString())
            name += QString(" (%1)").arg(stopDesc);
    }
    return name;
}

QString ParserFinlandMatka::timetableStationID(QString stationIDFromSearchResult,
                                               Mode mode)
{
    stationIDFromSearchResult.replace(QRegExp("^GTFS:"), "");
    stationIDFromSearchResult.replace(QRegExp("^TAMPERE:"), "JOLI:");

    // The geocoding API seems to filter out all but one node with the same position.
    // Many airports (e.g. Vaasa) are represented by one stop for departures and
    // one stop for arrivals with exactly the same position (e.g. MATKA:30281_ARRIVAL_EFVA
    // and MATKA:30281_EFVA). Since all airports ID's seem to start with MATKA:30281,
    // we can identify airports ID's that should be changed to match arrivals
    // or departures. A workaround for bad API's...
    static const QRegExp arr_airport_expr = QRegExp("(^MATKA:30281_)ARRIVAL_([A-Z]+)$");
    static const QRegExp dep_airport_expr = QRegExp("(^MATKA:30281_)([A-Z]+)$");
    if (mode == Departure && arr_airport_expr.exactMatch(stationIDFromSearchResult)) {
        qDebug() << "Changing arrival airport stop to departure";
        qDebug() << "Before:" << stationIDFromSearchResult;
        stationIDFromSearchResult.replace(arr_airport_expr, "\\1\\2");
        qDebug() << "After:" << stationIDFromSearchResult;
    } else if (mode == Arrival && dep_airport_expr.exactMatch(stationIDFromSearchResult)){
        qDebug() << "Changing departure airport stop to arrival";
        qDebug() << "Before:" << stationIDFromSearchResult;
        stationIDFromSearchResult.replace(dep_airport_expr, "\\1ARRIVAL_\\2");
        qDebug() << "After:" << stationIDFromSearchResult;
    }

    return stationIDFromSearchResult;
}

QString ParserFinlandMatka::prettyPlaceType(const QString& type)
{
    if (type == "localadmin" || type == "locality" || type == "county" || type == "neighbourhood")
        return tr("Place");
    else if (type == "station")
        return tr("Station");
    else if (type == "stop")
        return tr("Stop");
    else if (type == "region")
        return tr("Region");
    else
        return "";
}

QString ParserFinlandMatka::languageCode() const
{
    if (QLocale().language() == QLocale::Swedish)
        return "sv";
    else
        return "fi";
}
