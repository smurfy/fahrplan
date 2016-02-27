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
#endif
#include <QDomDocument>
#include <QLocale>
#include <QRegExp>
#include <algorithm>

#include "parser_finland_matka.h"

static const QString STATION_TYPE_STOP = "10";
static const QString STATION_TYPE_POI_ACCOMODATION = "100";
static const QString STATION_TYPE_POI_OTHER = "102";
static const QString STATION_TYPE_POI_SCHOOL = "103";
static const QString STATION_TYPE_POI_HOSPITAL = "104";
static const QString STATION_TYPE_POI_TRANSPORTATION = "105";
static const QString STATION_TYPE_POI_DISTRICT = "106";
static const QString STATION_TYPE_POI_MISC = "108";
static const QString STATION_TYPE_STREET = "900";

ParserFinlandMatka::ParserFinlandMatka(QObject *parent) :
        ParserAbstract(parent),
        baseURL(QLatin1String("http://api.matka.fi/")),
        username(QLatin1String("fahrplan")),
        password(QLatin1String("dPr263pJVKQRly0VpsiK"))
{
    setupCityNames();
}

bool ParserFinlandMatka::supportsGps()
{
    return true;
}

bool ParserFinlandMatka::supportsVia()
{
    return false;
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

void ParserFinlandMatka::findStationsByName(const QString &stationName)
{
    if (stationName.length() < 2)
        return;
    if (currentRequestState != FahrplanNS::noneRequest)
        return;
    currentRequestState = FahrplanNS::stationsByNameRequest;

    QUrl url(baseURL);
#if defined(BUILD_FOR_QT5)
    QUrlQuery query;
#else
    QUrl query;
#endif
    query.addQueryItem("user", username);
    query.addQueryItem("pass", password);
    query.addQueryItem("key", toPercentEncoding(stationName));
#if defined(BUILD_FOR_QT5)
    url.setQuery(query);
#else
    url.setQueryItems(query.queryItems());
#endif
    qDebug() << "Sending station request (by name):" << url.toString();
    sendHttpRequest(url);
}

void ParserFinlandMatka::parseStationsByName(QNetworkReply *networkReply)
{
    StationsList result;
    QDomDocument doc("result");
    if (doc.setContent(networkReply->readAll(), false)) {
        qDebug() << doc.toString(2);
        const QDomNodeList& locations(doc.elementsByTagName("LOC"));
        for (int i = 0; i < locations.count(); ++i) {
            const QDomNode& location = locations.at(i);
            Station station;
            station.name = getAttribute(location, "name1");
            const QString number(getAttribute(location, "number"));
            if (!number.isEmpty())
                station.name += " " + number;
            const QString city(getAttribute(location, "city"));
            if (!city.isEmpty() && city != station.name)
                station.name += ", " + city;
            double lat;
            double lon;
            projection.gridToGeodetic(getAttribute(location, "y").toDouble(),
                                      getAttribute(location, "x").toDouble(),
                                      lat, lon);
            station.longitude = lon;
            station.latitude = lat;
            station.type = getAttribute(location, "type");
            station.miscInfo = prettyStationType(station.type);

            // The "code" attribute is not a reliable ID on it's own and can sometimes be empty,
            // sometimes same for many locations (for example street results).
            // Instead use the name + type + code as ID.
            station.id = station.name + ";" + station.type + ";" + getAttribute(location, "code");

            result << station;
        }
        std::sort(result.begin(), result.end(), sortUtility);
    }

    emit stationsResult(result);
}

void ParserFinlandMatka::findStationsByCoordinates(qreal longitude, qreal latitude)
{
    if (currentRequestState != FahrplanNS::noneRequest)
        return;
    currentRequestState = FahrplanNS::stationsByCoordinatesRequest;

    double x;
    double y;
    projection.geodeticToGrid(latitude, longitude, y, x);
    QUrl url(baseURL + "timetables/");
#if defined(BUILD_FOR_QT5)
    QUrlQuery query;
#else
    QUrl query;
#endif
    query.addQueryItem("user", username);
    query.addQueryItem("pass", password);
    query.addQueryItem("m", "stop");
    query.addQueryItem("x", QString::number(x, 'f', 0));
    query.addQueryItem("y", QString::number(y, 'f', 0));
    query.addQueryItem("radius", "1000");
    query.addQueryItem("count", "30");
#if defined(BUILD_FOR_QT5)
    url.setQuery(query);
#else
    url.setQueryItems(query.queryItems());
#endif
    qDebug() << "Sending station request (by coordinates):" << url.toString();
    sendHttpRequest(url);
}

void ParserFinlandMatka::parseStationsByCoordinates(QNetworkReply *networkReply)
{
    StationsList result;
    QDomDocument doc("result");
    if (doc.setContent(networkReply->readAll(), false)) {
        qDebug() << doc.toString(2);
        const QDomNodeList& stops(doc.elementsByTagName("STOP"));
        for (int i = 0; i < stops.size(); ++i) {
            const QDomNode& stop = stops.at(i);
            Station station;
            station.name = timetableStopName(stop.toElement());
            bool hasCityID;
            int cityID = getAttribute(stop, "cityId").toInt(&hasCityID);
            const QString city(cityName(cityID));
            if (hasCityID && city != station.name)
                station.name += ", " + city;
            double lat;
            double lon;
            projection.gridToGeodetic(getAttribute(stop, "yCoord").toDouble(),
                                      getAttribute(stop, "xCoord").toDouble(),
                                      lat, lon);
            station.longitude = lon;
            station.latitude = lat;
            station.type = STATION_TYPE_STOP;
            station.miscInfo = prettyStationType(station.type);

            // The "code" attribute is not a reliable ID on it's own and can sometimes be empty,
            // sometimes same for many locations (for example street results).
            // Instead use the name + type + code as ID.
            station.id = station.name + ";" + station.type + ";" + getAttribute(stop, "code");

            result << station;
        }
    }

    emit stationsResult(result);
}

/**
 * The matka.fi API doesn't return all data we need to create the timetable in one go,
 * so three types of requests have to be made:
 *   - Find stops in the area around the stop in the original request. This is necessary
 *     since stops found using the name based search only returns one of the several
 *     stop locations with that name in the area.
 *   - For each stop matching the name of the original stop, get a list of lines
 *     that departure/arrive that stop.
 *   - For each line, download the line description which we need to find out the
 *     destination station. The list of lines retrieved in the previous step certainly
 *     contains a string like for example "Leppävaara-Kolmperä-Siikajärvi-Siikaniemi",
 *     but it is the same for both directions of the line, so it is not possible to find
 *     out the final destination using it in a reliable way.
 *
 * The different requests are all using the timetable request/parsing methods, so to keep
 * track of the currently ongoing request type we use the variable timetableRequestState.
 * Ultimately, it would be better to not misuse the methods like this, but until there is
 * a better way, for example by letting the parser register custom request types that are
 * recognized by ParserAbstract::networkReplyFinished(), this at least seems to work quite
 * OK.
 */
void ParserFinlandMatka::getTimeTableForStation(const Station &currentStation,
                                            const Station &directionStation,
                                            const QDateTime &dateTime,
                                            Mode mode,
                                            int trainrestrictions)
{
    if (currentStation.type != STATION_TYPE_STOP) {
        emit errorOccured(tr("Timetables are only available for stops."));
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

    QUrl url(baseURL + "timetables/");
#if defined(BUILD_FOR_QT5)
    QUrlQuery query;
#else
    QUrl query;
#endif
    query.addQueryItem("user", username);
    query.addQueryItem("pass", password);
    if (!timetableStopsToDownload.isEmpty()) {
        timetableRequestState = TimetableStopRequest;
        query.addQueryItem("m", "stopid"); // Search for timetables for a stop
        query.addQueryItem("stopid", timetableStopsToDownload.takeFirst());
        query.addQueryItem("time", dateTime.time().toString("hhmm"));
        query.addQueryItem("date", dateTime.date().toString("yyyyMMdd"));
        query.addQueryItem("count", "30"); // Number of timetable entries to return (valid values: 1-1000)
        qDebug() << "Sending timetable request";
    } else if (!linesWaitingForDownload.isEmpty()) {
        timetableRequestState = TimetableLineRequest;
        QString id(*(linesWaitingForDownload.begin()));
        linesWaitingForDownload.erase(linesWaitingForDownload.begin());
        query.addQueryItem("m", "lineid");
        query.addQueryItem("lineid", id);
    } else if (timetable.isEmpty()) {
        timetableRequestState = TimetableAreaRequest;
        query.addQueryItem("m", "stop"); // Search for stops in an area
        double x;
        double y;
        projection.geodeticToGrid(currentStation.latitude, currentStation.longitude, y, x);
        query.addQueryItem("x", QString::number(x, 'f', 0));
        query.addQueryItem("y", QString::number(y, 'f', 0));
        query.addQueryItem("count", "50");
        query.addQueryItem("radius", "1000");
        qDebug() << "Sending request to find the rest of the stops around" << currentStation.name;
    } else {
        finishTimetable();
        currentRequestState = FahrplanNS::noneRequest;
        timetableRequestState = NoTimetableRequest;
        return;
    }
#if defined(BUILD_FOR_QT5)
    url.setQuery(query);
#else
    url.setQueryItems(query.queryItems());
#endif

    qDebug() << "URL:" << url.toString();
    sendHttpRequest(url);
}

void ParserFinlandMatka::parseTimeTable(QNetworkReply *networkReply)
{
    bool success = false;
    switch (timetableRequestState) {
    case TimetableAreaRequest:
        success = parseTimetableAreaInfo(networkReply);
        break;
    case TimetableStopRequest:
        success = parseTimetableStopInfo(networkReply);
        break;
    case TimetableLineRequest:
        success = parseTimetableLineInfo(networkReply);
        break;
    default:
        qDebug() << "Not a valid timetable request state:" << timetableRequestState;
        emit errorOccured(tr("Failed to parse timetable data."));
        break;
    }
    timetableRequestState = NoTimetableRequest;
    if (success) {
        if (timetable.isEmpty() && timetableStopsToDownload.isEmpty()) {
            emit timetableResult(TimetableEntriesList());
        } else {
            getTimeTableForStation(lastTimetableSearch.currentStation,
                                   lastTimetableSearch.directionStation,
                                   lastTimetableSearch.dateTime,
                                   lastTimetableSearch.mode,
                                   lastTimetableSearch.restrictions);
        }
    } else {
        emit errorOccured(tr("Failed to parse timetable data."));
    }
}

bool ParserFinlandMatka::parseTimetableAreaInfo(QNetworkReply *networkReply)
{
    QDomDocument doc("result");
    if (doc.setContent(networkReply->readAll(), false)) {
        qDebug() << doc.toString(2);
        const QDomNodeList& stops(doc.elementsByTagName("STOP"));
        if (stops.isEmpty()) {
            qDebug() << "No stops found around " << lastTimetableSearch.currentStation.name;
            return false;
        }
        if (!lastTimetableSearch.currentStation.name.isEmpty()) {
            // Remove the city part from the stop name to more easily compare to the stop names in the list
            QString searchedStopName(lastTimetableSearch.currentStation.name.split(',').first().trimmed());
            timetableStopsToDownload.clear();
            timetable.clear();
            lastTimetableSearch.stopsMatchingCurrentStation.clear();
            for (int i = 0; i < stops.size(); ++i) {
                const QDomNode& stop = stops.at(i);
                QString comparableStopName(timetableStopName(stop.toElement(), false).split(',').first().trimmed());
                if (comparableStopName == searchedStopName) {
                    QString stopID = getAttribute(stop, "id");
                    qDebug() << QString("Adding stop with id %1 for %2").arg(stopID).arg(lastTimetableSearch.currentStation.name);
                    if (stopID.isEmpty()) {
                        qDebug() << "No valid stop ID found for this stop";
                        return false;
                    }
                    timetableStopsToDownload << stopID;
                    lastTimetableSearch.stopsMatchingCurrentStation.insert(stopID);
                }
            }
            return true;
        } else {
            qDebug() << "No name for the searched stop found - shouldn't happen";
            return false;
        }
    } else {
        qDebug() << "Failed to read network reply data";
        return false;
    }
}

bool ParserFinlandMatka::parseTimetableStopInfo(QNetworkReply *networkReply)
{
    QDomDocument doc("result");
    if (doc.setContent(networkReply->readAll(), false)) {
        qDebug() << doc.toString(2);
        const QDomNodeList& stops(doc.elementsByTagName("STOP"));
        for (int i = 0; i < stops.size(); ++i) {
            const QDomNodeList& lineNodes(stops.at(i).toElement().elementsByTagName("LINE"));
            for (int j = 0; j < lineNodes.count(); ++j) {
                const QDomNode& lineNode = lineNodes.at(j);
                bool transportTypeOK;
                int transportType = getAttribute(lineNode, "transportType").toInt(&transportTypeOK);
                if (transportTypeOK && !transportTypeIsSelected(transportType, lastTimetableSearch.restrictions))
                    continue;
                SortableTimetableEntry sortableEntry;
                TimetableEntry& entry(sortableEntry.entry);
                entry.currentStation = lastTimetableSearch.currentStation.name;
                localizeCityName(entry.currentStation);
                sortableEntry.lineID = getAttribute(lineNode, "id");
                if (!sortableEntry.lineID.isEmpty() && !lineStops.contains(sortableEntry.lineID))
                    linesWaitingForDownload.insert(sortableEntry.lineID);
                entry.latitude = lastTimetableSearch.currentStation.latitude;
                entry.longitude = lastTimetableSearch.currentStation.longitude;
                QString timeAttr = "departureTime";
                if (lastTimetableSearch.mode == Arrival)
                    timeAttr = "arrivalTime";
                entry.time = QTime::fromString(getAttribute(lineNode, timeAttr), "hhmm");
                sortableEntry.dateTime.setTime(entry.time);
                if (entry.time >= lastTimetableSearch.dateTime.time())
                    sortableEntry.dateTime.setDate(lastTimetableSearch.dateTime.date());
                else
                    sortableEntry.dateTime.setDate(lastTimetableSearch.dateTime.date().addDays(1));
                if (transportTypeOK)
                    entry.trainType = transportTypeName(transportType);
                QString lineCode = getAttribute(lineNode, "code");
                if (!lineCode.isEmpty() && lineCode != "-")
                    entry.trainType += " " + lineCode;
                timetable << sortableEntry;
            }
        }
        return true;
    } else {
        qDebug() << "Failed to read network reply data";
        return false;
    }
}

bool ParserFinlandMatka::parseTimetableLineInfo(QNetworkReply *networkReply)
{
    QDomDocument doc("result");
    if (doc.setContent(networkReply->readAll(), false)) {
        qDebug() << doc.toString(2);
        const QDomNodeList& stopLists(doc.elementsByTagName("LINE2STOPS"));
        if (stopLists.size() != 1) {
            qDebug() << "Expected exactly one LINE2STOPS element";
            return false;
        }
        const QDomElement& stopList(stopLists.at(0).toElement());
        QString lineID(getAttribute(stopList, "lineId"));
        if (lineID.isEmpty()) {
            qDebug() << "No line ID found in the LINE2STOPS response";
            return false;
        }
        const QDomNodeList& stops(stopList.elementsByTagName("STOP"));
        if (stops.isEmpty()) {
            qDebug() << "No stops found for line" << lineID;
            return false;
        }

        QStringList stopIDs;
        for (int i = 0; i < stops.size(); ++i) {
            const QDomElement& stop = stops.at(i).toElement();
            QString id(getAttribute(stop, "id"));
            stopIDs << id;
            stopNames.insert(id, timetableStopName(stop));
        }
        lineStops.insert(lineID, stopIDs);
        return true;
    } else {
        qDebug() << "Failed to read network reply data";
        return false;
    }
}

void ParserFinlandMatka::finishTimetable()
{
    std::sort(timetable.begin(), timetable.end(), sortUtility);

    QString directionStop;
    if (lastTimetableSearch.directionStation.valid)
        directionStop = lastTimetableSearch.directionStation.name;

    QString currentStop(lastTimetableSearch.currentStation.name);

    // Remove any city part from the stop names to make them easier to compare
    if (!directionStop.isEmpty())
        directionStop = directionStop.split(',').first().trimmed();
    if (!currentStop.isEmpty())
        currentStop = currentStop.split(',').first().trimmed();

    TimetableEntriesList result;
    QString lastLineID;
    QDateTime lastDateTime;
    foreach (SortableTimetableEntry entry, timetable)
    {
        // Filter out duplicate entries
        // Some stops seem to list some departures several times, e.g. stop 8619151 lists
        // line 11816242 departuring 05:29 twice. Perhaps the server-side search includes
        // results from after the next 24 hours without indicating the day.
        if (!lastLineID.isEmpty() && lastDateTime.isValid() &&
                entry.lineID == lastLineID && entry.dateTime == lastDateTime) {
            qDebug() << "Skipping duplicate entry:" << lastLineID << lastDateTime;
            continue;
        }
        lastLineID = entry.lineID;
        lastDateTime = entry.dateTime;

        const QStringList& stopsAlongLine(lineStops.value(entry.lineID));

        // Filter out departures/arrivals based on the searched direction station
        if (!directionStop.isEmpty()) {
            QList<QString>::const_iterator it = stopsAlongLine.begin();
            while (it != stopsAlongLine.end() && stopNames.value(*it) != currentStop)
                ++it;
            bool directionStopIsAfterCurrentStop = false;
            while (it != stopsAlongLine.end()) {
                if (stopNames.value(*it) == directionStop) {
                    directionStopIsAfterCurrentStop = true;
                    break;
                }
                ++it;
            }
            if (!directionStopIsAfterCurrentStop)
                continue;
        }

        // Assign each timetable entry a destination/origin station
        if (stopsAlongLine.size() >= 2) {
            if (lastTimetableSearch.mode == Arrival) {
                const QString& fromStop(stopsAlongLine.first());
                if (lastTimetableSearch.stopsMatchingCurrentStation.contains(fromStop)) {
                    qDebug() << QString("Skipping arrival for which the \"from\" station (%1) is "
                                        "the \"current station\" (line ID: %2)").arg(fromStop).arg(entry.lineID);
                    continue;
                } else {
                    entry.entry.destinationStation = stopNames.value(fromStop);
                }
            } else {
                const QString& toStop(stopsAlongLine.last());
                if (lastTimetableSearch.stopsMatchingCurrentStation.contains(toStop)) {
                    qDebug() << QString("Skipping departure for which the \"to\" station (%1) is "
                                        "the \"current station\" (line ID: %2)").arg(toStop).arg(entry.lineID);
                    continue;
                } else {
                    entry.entry.destinationStation = stopNames.value(toStop);
                }
            }
        }
        result << entry.entry;
    }
    timetable.clear();
    emit timetableResult(result);
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

void ParserFinlandMatka::internalSearchJourney(const Station &departureStation, const Station &viaStation,
                                               const Station &arrivalStation, const QDateTime &dateTime,
                                               Mode mode, int trainRestrictions)
{
    Q_UNUSED(viaStation)

    lastJourneySearch.dateTime = dateTime;
    lastJourneySearch.from = departureStation;
    lastJourneySearch.to = arrivalStation;
    lastJourneySearch.restrictions = trainRestrictions;
    lastJourneySearch.mode = mode;
    lastJourneySearch.resultCount = 0;

    QUrl url(baseURL);
#if defined(BUILD_FOR_QT5)
    QUrlQuery query;
#else
    QUrl query;
#endif
    query.addQueryItem("user", username);
    query.addQueryItem("pass", password);
    double x;
    double y;
    projection.geodeticToGrid(departureStation.latitude, departureStation.longitude, y, x);
    query.addQueryItem("a", QString("%1,%2")
                       .arg(QString::number(x, 'f', 0))
                       .arg(QString::number(y, 'f', 0)));
    projection.geodeticToGrid(arrivalStation.latitude, arrivalStation.longitude, y, x);
    query.addQueryItem("b", QString("%1,%2")
                       .arg(QString::number(x, 'f', 0))
                       .arg(QString::number(y, 'f', 0)));
    query.addQueryItem("time", dateTime.time().toString("hhmm"));
    query.addQueryItem("date", dateTime.date().toString("yyyyMMdd"));
    if (mode == Arrival)
        query.addQueryItem("timemode", "2");
    else
        query.addQueryItem("timemode", "1");
    query.addQueryItem("show", "5"); // Number of search results (valid values: 1, 3 or 5)
#if defined(BUILD_FOR_QT5)
    url.setQuery(query);
#else
    url.setQueryItems(query.queryItems());
#endif
    qDebug() << "Sending journey request:" << url.toString();
    sendHttpRequest(url);
}

void ParserFinlandMatka::parseSearchJourney(QNetworkReply *networkReply)
{
    JourneyResultList *journeyList = new JourneyResultList();
    QDomDocument doc("result");
    if (doc.setContent(networkReply->readAll(), false)) {
        qDebug() << doc.toString(2);
        const QDomNodeList& errors(doc.elementsByTagName("ERROR"));
        if (errors.size() > 0)
            qDebug() << "Error:" << errors.at(0).toText().data();
        const QDomNodeList& routes(doc.elementsByTagName("ROUTE"));
        int journeyCounter = 0;
        bool hasFoundWalkOnlyRoute = false;
        for (int i = 0; i < routes.count(); ++i) {
            QDomElement route;
            // If the search was on arrival the routes are returned in the reverse order
            if (lastJourneySearch.mode == Arrival)
                route = routes.at(routes.count()-1-i).toElement();
            else
                route = routes.at(i).toElement();

            QString journeyID = QString::number(journeyCounter);
            double walkingDistance = 0.0;
            QList<JourneyDetailResultItem*> segments = parseJourneySegments(route, walkingDistance);
            if (segments.isEmpty())
                continue;

            int minutes = segments.first()->departureDateTime().secsTo(segments.last()->arrivalDateTime()) / 60;
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
            journey->setDuration(duration);
            journey->setTransfers(QString::number(transportModes.count()-1));
            if (walkingDistance > 0.0)
                journey->setMiscInfo(tr("Walk %1").arg(formatDistance(walkingDistance)));
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
    }
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

QList<JourneyDetailResultItem*> ParserFinlandMatka::parseJourneySegments(
        const QDomElement& journeyData, double& totalWalkingDistance)
{
    QList<JourneyDetailResultItem*> results;

    for (int i = 0; i != journeyData.childNodes().count(); ++i) {
        const QDomNode& n = journeyData.childNodes().at(i);
        if (!n.isElement())
            continue;
        qDebug() << "Parsing journey";
        const QDomElement& elem = n.toElement();
        double distance = 0.0;
        if (elem.nodeName() == "LINE" || elem.nodeName() == "WALK") {
            JourneyDetailResultItem* resultItem = new JourneyDetailResultItem();
            bool isWalk = elem.nodeName() == "WALK";
            JourneySegmentParseResult result = parseJourneySegment(elem, isWalk, distance, resultItem);
            if (result == JourneySegmentOK) {
                if (isWalk) {
                    resultItem->setInternalData1("WALK");
                    totalWalkingDistance += distance;
                }
                results << resultItem;
            } else if (result == SkipJourney) {
                foreach (JourneyDetailResultItem* item, results) {
                    delete item;
                    item = NULL;
                }
                return QList<JourneyDetailResultItem*>();
            } else {
                delete resultItem;
            }
        }
    }
    return results;
}

ParserFinlandMatka::JourneySegmentParseResult ParserFinlandMatka::parseJourneySegment(
        const QDomElement& segmentData, bool isWalk, double& distance,
        JourneyDetailResultItem* journeySegment)
{
    QString firstPointX, firstPointY, lastPointX, lastPointY;
    QString firstPointCity, lastPointCity, depName, arrName, unlocalizedDepName, unlocalizedArrName;
    bool isFirstSegment = false;
    bool isLastSegment = false;

    const QDomElement& lengthElem = segmentData.firstChildElement("LENGTH");
    if (lengthElem.hasAttribute("dist")) {
        bool ok;
        distance = lengthElem.attribute("dist").toDouble(&ok);
        if (ok)
            journeySegment->setInfo(formatDistance(distance));
    }
    for (int i = 0; i < segmentData.childNodes().count(); ++i) {
        const QDomElement& elem = segmentData.childNodes().at(i).toElement();
        if (!elem.isNull() && (elem.nodeName() == "STOP" ||
                               elem.nodeName() == "MAPLOC" ||
                               elem.nodeName() == "POINT")) {
            depName = placeName(elem);
            unlocalizedDepName = placeName(elem, false);
            if (getAttribute(elem, "uid") == "start")
                isFirstSegment = true;
            if (depName.isEmpty() && isFirstSegment) {
                QString name(lastJourneySearch.from.name);
                localizeCityName(name);
                journeySegment->setDepartureStation(name);
            } else {
                journeySegment->setDepartureStation(depName);
            }
            const QDomElement& timeElem = elem.firstChildElement("DEPARTURE");
            QDateTime dt;
            dt.setTime(QTime::fromString(timeElem.attribute("time"), "hhmm"));
            dt.setDate(QDate::fromString(timeElem.attribute("date"), "yyyyMMdd"));
            journeySegment->setDepartureDateTime(dt);
            firstPointX = getAttribute(elem, "x");
            firstPointY = getAttribute(elem, "y");
            firstPointCity = cityName(elem, false);
            break;
        }
    }
    for (int i = segmentData.childNodes().count() - 1; i >= 0; --i) {
        const QDomElement& elem = segmentData.childNodes().at(i).toElement();
        if (!elem.isNull() && (elem.nodeName() == "STOP" ||
                               elem.nodeName() == "MAPLOC" ||
                               elem.nodeName() == "POINT")) {
            if (getAttribute(elem, "uid") == "dest")
                isLastSegment = true;
            arrName = placeName(elem);
            unlocalizedArrName = placeName(elem, false);
            if (arrName.isEmpty() && isLastSegment) {
                QString name(lastJourneySearch.to.name);
                localizeCityName(name);
                journeySegment->setArrivalStation(name);
            } else {
                journeySegment->setArrivalStation(arrName);
            }
            const QDomElement& timeElem = elem.firstChildElement("ARRIVAL");
            QDateTime dt;
            dt.setTime(QTime::fromString(timeElem.attribute("time"), "hhmm"));
            dt.setDate(QDate::fromString(timeElem.attribute("date"), "yyyyMMdd"));
            journeySegment->setArrivalDateTime(dt);
            lastPointX = getAttribute(elem, "x");
            lastPointY = getAttribute(elem, "y");
            lastPointCity = cityName(elem, false);
            break;
        }
    }

    // If the first position is the same as the last, skip this segment
    // (yes, there are such segments for some reason)
    if ((isFirstSegment || isLastSegment) &&
            firstPointX == lastPointX &&
            firstPointY == lastPointY) {
        qDebug() << "Skipping segment (starts and ends in same position)";
        return SkipJourneySegment;
    }

    if (isWalk) {
        journeySegment->setTrain(tr("Walk"));

        // If the route starts or ends with a district, the user probably is satisfied
        // with getting to that city, rather then having an extra walk to the position
        // that happens to be the center of that district.
        // For example, if the user has searched for a route to Jyväskylä (district),
        // the second last segment may end at Jyväskylä Matkakeskus, and the last segment
        // is a 468 m walking trip to Jyväskylä. The code below filters out that last
        // segment.
        if ((isFirstSegment &&
                lastJourneySearch.from.type == STATION_TYPE_POI_DISTRICT &&
                lastPointCity == lastJourneySearch.from.name) ||
            (isLastSegment &&
                lastJourneySearch.to.type == STATION_TYPE_POI_DISTRICT &&
                firstPointCity == lastJourneySearch.to.name))
        {
            qDebug() << "Skipping segment (already in city)";
            return SkipJourneySegment;
        }

        // Each stop seems to have a corresponding stop for lines in the opposite direction
        // and only one of the stops appear when searching for stops. If the route departs
        // from the "other" stop (and not from the one selected as "From station"), the
        // first route segment is a short walk between the stops. The following code filters
        // out that unnecessary segment (and correspondingly for last segments).
        if ((isFirstSegment &&
                lastJourneySearch.from.type == STATION_TYPE_STOP &&
                unlocalizedArrName == lastJourneySearch.from.name) ||
            (isLastSegment &&
                lastJourneySearch.to.type == STATION_TYPE_STOP &&
                unlocalizedDepName == lastJourneySearch.to.name))
        {
            qDebug() << "Skipping segment (other stop with same name)";
            return SkipJourneySegment;
        }

    } else {
        bool transportTypeOK;
        int transportType = segmentData.attribute("type").toInt(&transportTypeOK);
        QString transportTypeString(transportTypeName(transportType));
        QString lineCode = segmentData.attribute("code");
        qDebug() << "Transport type:" << segmentData.attribute("type") << "(as string:" << transportTypeString << ")";
        if (!transportTypeIsSelected(transportType, lastJourneySearch.restrictions)) {
            qDebug() << "This journey segment is using an unselected transport type - skipping journey";
            return SkipJourney;
        }
        qDebug() << "Line number:" << segmentData.attribute("code");
        QString transportString;
        if (transportTypeOK)
            transportString = transportTypeString;
        else
            transportString = tr("Invalid transport type: %1").arg(segmentData.attribute("type"));
        if (transportTypeOK && !lineCode.isEmpty() && lineCode != "-")
            transportString += " " + lineCode;
        journeySegment->setTrain(transportString);
    }
    return JourneySegmentOK;
}

void ParserFinlandMatka::getJourneyDetails(const QString &id)
{
    if (cachedResults.contains(id))
        emit journeyDetailsResult(cachedResults.value(id));
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
        //: Kilometers
        return QString("%1 %2").arg(QString::number(distance/1000.0, 'f', 0)).arg(tr("km"));
    if (distance > 1000)
        return QString("%1 %2").arg(QString::number(distance/1000.0, 'f', 1)).arg(tr("km"));
    else
        //: Meters
        return QString("%1 %2").arg(QString::number(distance, 'f', 0)).arg(tr("m"));
}

/**
 * This is like QUrl::toPercentEncoding() but instead of UTF-8 it uses Latin 1
 * (ISO-8859-1) which is required by the API. Perhaps there exists a prettier
 * way to do it, but this function gets the job done.
 *
 * Example: "vöyri" is converted to "v%F6yri"
 *          (QUrl::toPercentEncoding() would give "v%C3%B6yri")
 */
QByteArray ParserFinlandMatka::toPercentEncoding(const QString& string)
{
    QByteArray ba1(string.toLatin1());
    QByteArray ba2;
    foreach (unsigned char c, ba1) {
        if (c > 0x7F)
            ba2.append(QString().sprintf("%%%02X", c));
        else
            ba2.append(c);
    }
    return ba2;
}

QString ParserFinlandMatka::transportTypeName(int code)
{
    switch (code) {
    case ExpressTrain: return tr("Express train");
    case Tolstoi: return "Tolstoi";
    case Allegro: return "Allegro";
    case InterCity: return "InterCity";
    case InterCity2: return "InterCity2";
    case RegionalTrain: return tr("Regional train");
    case TrainBus: return tr("Train bus");
    case PohjolanLiikenne: return "Pohjan Liikenne";
    case CommuterTrain1:
    case CommuterTrain2: return tr("Commuter train");
    case Pendolino: return "Pendolino";
    case BusValluExpress:
    case BusVRExpress: return tr("Express bus");
    case Taxi: return tr("Taxi");
    case ConnectionBus1:
    case ConnectionBus2: return tr("Connection bus");
    case BusTKLVakio:
    case BusValluVakio1:
    case BusValluVakio2:
    case BusVRVakio: return tr("Local bus");
    case BusJKL:
    case BusKaj:
    case BusKVL:
    case BusOul:
    case BusRae:
    case BusSHJoe:
    case BusSHKot:
    case BusSHKuo:
    case BusSHLPR:
    case BusSHMKL:
    case BusSHOul:
    case BusSHVaa:
    case BusTKL1:
    case BusTKL2:
    case BusTur1:
    case BusTur2:
    case BusValluLinjausok1:
    case BusValluLinjausok2:
    case BusValluPika:
    case BusVRPika:
    case BusYTV1:
    case BusYTV2:
    case BusYTV3:
    case BusYTV4:
    case BusYTV5:
    case BusYTV6:
    case BusYTV7:
    case BusYTV8:
    case BusYTV9:
    case BusYTV10:
    case BusYTV11:
    case BusYTV12:
    case BusYTV13:
    case BusSK: return tr("Bus");
    case KirkkonummiCityBus:
    case SipooCityBus:
    case KeravaCityBus: return tr("City bus");
    case Air:
        //: Used in conjunction with a flight number, e.g. "Flight D1234"
        return tr("Flight");
    case CarValluVakio:
    case CarLinjausok: return tr("Car");
    case Tram: return tr("Tram");
    case Metro: return tr("Metro");
    case Ferry: return tr("Ferry");
    case TrainYTV1:
    case TrainYTV2: return tr("Train");
    default: return tr("Unknown transport type: %1").arg(code);
    }
}

bool ParserFinlandMatka::transportTypeIsSelected(int transportType, int selectedTransportModeFilter)
{
    qDebug() << "Checking if" << transportTypeName(transportType) << "is selected...";
    bool busSelected = false;
    bool trainSelected = false;
    bool airplaneSelected = false;
    switch (selectedTransportModeFilter) {
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

    switch (transportType) {
    case BusJKL:
    case BusKaj:
    case BusKVL:
    case BusOul:
    case BusRae:
    case BusSHJoe:
    case BusSHKot:
    case BusSHKuo:
    case BusSHLPR:
    case BusSHMKL:
    case BusSHOul:
    case BusSHVaa:
    case BusSK:
    case BusTur1:
    case BusTur2:
    case BusTKL1:
    case BusTKL2:
    case BusTKLVakio:
    case BusValluExpress:
    case BusValluLinjausok1:
    case BusValluLinjausok2:
    case BusValluPika:
    case BusValluVakio1:
    case BusValluVakio2:
    case BusVRExpress:
    case BusVRPika:
    case BusVRVakio:
    case BusYTV1:
    case BusYTV2:
    case BusYTV3:
    case BusYTV4:
    case BusYTV5:
    case BusYTV6:
    case BusYTV7:
    case BusYTV8:
    case BusYTV9:
    case BusYTV10:
    case BusYTV11:
    case BusYTV12:
    case BusYTV13:
    case ConnectionBus1:
    case ConnectionBus2:
    case KeravaCityBus:
    case KirkkonummiCityBus:
    case PohjolanLiikenne:
    case SipooCityBus:
    case TrainBus:
        return busSelected;
    case Allegro:
    case CommuterTrain1:
    case CommuterTrain2:
    case ExpressTrain:
    case InterCity:
    case InterCity2:
    case Pendolino:
    case RegionalTrain:
    case Tolstoi:
    case TrainYTV1:
    case TrainYTV2:
        return trainSelected;
    case Air:
        return airplaneSelected;
    default:
        // Tram, metro, ferry, taxi, car
        return true;
    }
}

QString ParserFinlandMatka::prettyStationType(const QString& type)
{
    if (type == STATION_TYPE_STOP)
        return tr("Stop");
    else if (type == STATION_TYPE_POI_ACCOMODATION)
        return tr("Accomodation");
    else if (type == STATION_TYPE_POI_MISC || type == STATION_TYPE_POI_OTHER)
        return tr("POI");
    else if (type == STATION_TYPE_POI_SCHOOL)
        return tr("School");
    else if (type == STATION_TYPE_POI_HOSPITAL)
        return tr("Hospital");
    else if (type == STATION_TYPE_POI_TRANSPORTATION)
        return tr("Transport POI");
    else if (type == STATION_TYPE_POI_DISTRICT)
        return tr("District");
    else if (type == STATION_TYPE_STREET)
        return tr("Street");
    else
        return "";
}

QString ParserFinlandMatka::placeName(const QDomElement& place, bool localize)
{
    QDomNodeList names = place.elementsByTagName("NAME");
    QString name;
    QString swedishName;
    for (int i = 0; i < names.count(); ++i) {
        const QDomElement& nameElem = names.at(i).toElement();
        if (nameElem.attribute("lang", "1") == "2")
            swedishName = nameElem.attribute("val");
        else
            name = nameElem.attribute("val");
    }
    if (localize && QLocale().language() == QLocale::Swedish) {
        if (swedishName.isEmpty())
            localizeCityName(name);
        else
            name = swedishName;
    }

    // Find the city name, if any
    QString city(cityName(place, localize));
    if (!city.isEmpty() && city != name)
        name += ", " + city;

    return name;
}

QString ParserFinlandMatka::timetableStopName(const QDomElement& stopElement, bool localize)
{
    QDomNodeList names = stopElement.elementsByTagName("name");
    QString name;
    QString swedishName;
    for (int i = 0; i < names.count(); ++i) {
        const QDomElement& nameElem = names.at(i).toElement();
        if (nameElem.attribute("lang", "fi") == "se")
            swedishName = nameElem.text();
        else
            name = nameElem.text();
    }
    if (localize && QLocale().language() == QLocale::Swedish) {
        if (swedishName.isEmpty())
            localizeCityName(name);
        else
            name = swedishName;
    }

    return name;
}

QString ParserFinlandMatka::cityName(const QDomElement& element, bool localize)
{
    const QDomNodeList& extras = element.elementsByTagName("XTRA");
    for (int i = 0; i < extras.size(); ++i) {
        const QDomElement& extraElem = extras.at(i).toElement();
        if (extraElem.attribute("name") == "city_id") {
            bool ok;
            int cityID = extraElem.attribute("val").toInt(&ok);
            if (ok)
                return cityName(cityID, localize);
        }
    }
    return QString();
}

QString ParserFinlandMatka::cityName(int code, bool localize)
{
    QString name(cityNames.value(code, tr("Unknown city")));
    if (localize)
        localizeCityName(name);
    return name;
}

void ParserFinlandMatka::localizeCityName(QString& placeName)
{
    if (QLocale().language() == QLocale::Swedish) {
        placeName = finnishToSwedishCityNames.value(placeName, placeName);
        QRegExp expr("(.*,\\s*)(.*)$");
        if (expr.exactMatch(placeName)) {
            QString swedishCityName = finnishToSwedishCityNames.value(expr.cap(2), expr.cap(2));
            qDebug() << "Found match:" << expr.cap(1) << "," << expr.cap(2);
            placeName = placeName.replace(expr, "\\1" + swedishCityName);
        }
    }
}

void ParserFinlandMatka::setupCityNames()
{
    // Retrieved from http://tilastokeskus.fi/meta/luokitukset/kunta/001-2013/index_en.html
    // Note: There are newer versions, but the API still (January 2016) seems to use
    // the 2013 version (and that was also the version referred to when the support was asked
    // about the city/municipality names). Some municipalities seem to have disappeared since
    // then, but they are still used by the API (e.g. 532, Nastola).
    cityNames.insert(20, "Akaa");
    cityNames.insert(5, "Alajärvi");
    cityNames.insert(9, "Alavieska");
    cityNames.insert(10, "Alavus");
    cityNames.insert(16, "Asikkala");
    cityNames.insert(18, "Askola");
    cityNames.insert(19, "Aura");
    cityNames.insert(35, "Brändö");
    cityNames.insert(43, "Eckerö");
    cityNames.insert(46, "Enonkoski");
    cityNames.insert(47, "Enontekiö");
    cityNames.insert(49, "Espoo");
    cityNames.insert(50, "Eura");
    cityNames.insert(51, "Eurajoki");
    cityNames.insert(52, "Evijärvi");
    cityNames.insert(60, "Finström");
    cityNames.insert(61, "Forssa");
    cityNames.insert(62, "Föglö");
    cityNames.insert(65, "Geta");
    cityNames.insert(69, "Haapajärvi");
    cityNames.insert(71, "Haapavesi");
    cityNames.insert(72, "Hailuoto");
    cityNames.insert(74, "Halsua");
    cityNames.insert(75, "Hamina");
    cityNames.insert(76, "Hammarland");
    cityNames.insert(77, "Hankasalmi");
    cityNames.insert(78, "Hanko");
    cityNames.insert(79, "Harjavalta");
    cityNames.insert(81, "Hartola");
    cityNames.insert(82, "Hattula");
    cityNames.insert(86, "Hausjärvi");
    cityNames.insert(111, "Heinola");
    cityNames.insert(90, "Heinävesi");
    cityNames.insert(91, "Helsinki");
    cityNames.insert(97, "Hirvensalmi");
    cityNames.insert(98, "Hollola");
    cityNames.insert(99, "Honkajoki");
    cityNames.insert(102, "Huittinen");
    cityNames.insert(103, "Humppila");
    cityNames.insert(105, "Hyrynsalmi");
    cityNames.insert(106, "Hyvinkää");
    cityNames.insert(283, "Hämeenkoski");
    cityNames.insert(108, "Hämeenkyrö");
    cityNames.insert(109, "Hämeenlinna");
    cityNames.insert(139, "Ii");
    cityNames.insert(140, "Iisalmi");
    cityNames.insert(142, "Iitti");
    cityNames.insert(143, "Ikaalinen");
    cityNames.insert(145, "Ilmajoki");
    cityNames.insert(146, "Ilomantsi");
    cityNames.insert(153, "Imatra");
    cityNames.insert(148, "Inari");
    cityNames.insert(149, "Inkoo");
    cityNames.insert(151, "Isojoki");
    cityNames.insert(152, "Isokyrö");
    cityNames.insert(164, "Jalasjärvi");
    cityNames.insert(165, "Janakkala");
    cityNames.insert(167, "Joensuu");
    cityNames.insert(169, "Jokioinen");
    cityNames.insert(170, "Jomala");
    cityNames.insert(171, "Joroinen");
    cityNames.insert(172, "Joutsa");
    cityNames.insert(174, "Juankoski");
    cityNames.insert(176, "Juuka");
    cityNames.insert(177, "Juupajoki");
    cityNames.insert(178, "Juva");
    cityNames.insert(179, "Jyväskylä");
    cityNames.insert(181, "Jämijärvi");
    cityNames.insert(182, "Jämsä");
    cityNames.insert(186, "Järvenpää");
    cityNames.insert(202, "Kaarina");
    cityNames.insert(204, "Kaavi");
    cityNames.insert(205, "Kajaani");
    cityNames.insert(208, "Kalajoki");
    cityNames.insert(211, "Kangasala");
    cityNames.insert(213, "Kangasniemi");
    cityNames.insert(214, "Kankaanpää");
    cityNames.insert(216, "Kannonkoski");
    cityNames.insert(217, "Kannus");
    cityNames.insert(218, "Karijoki");
    cityNames.insert(224, "Karkkila");
    cityNames.insert(226, "Karstula");
    cityNames.insert(230, "Karvia");
    cityNames.insert(231, "Kaskinen");
    cityNames.insert(232, "Kauhajoki");
    cityNames.insert(233, "Kauhava");
    cityNames.insert(235, "Kauniainen");
    cityNames.insert(236, "Kaustinen");
    cityNames.insert(239, "Keitele");
    cityNames.insert(240, "Kemi");
    cityNames.insert(320, "Kemijärvi");
    cityNames.insert(241, "Keminmaa");
    cityNames.insert(322, "Kemiönsaari");
    cityNames.insert(244, "Kempele");
    cityNames.insert(245, "Kerava");
    cityNames.insert(249, "Keuruu");
    cityNames.insert(250, "Kihniö");
    cityNames.insert(256, "Kinnula");
    cityNames.insert(257, "Kirkkonummi");
    cityNames.insert(260, "Kitee");
    cityNames.insert(261, "Kittilä");
    cityNames.insert(263, "Kiuruvesi");
    cityNames.insert(265, "Kivijärvi");
    cityNames.insert(271, "Kokemäki");
    cityNames.insert(272, "Kokkola");
    cityNames.insert(273, "Kolari");
    cityNames.insert(275, "Konnevesi");
    cityNames.insert(276, "Kontiolahti");
    cityNames.insert(280, "Korsnäs");
    cityNames.insert(284, "Koski Tl");
    cityNames.insert(285, "Kotka");
    cityNames.insert(286, "Kouvola");
    cityNames.insert(287, "Kristiinankaupunki");
    cityNames.insert(288, "Kruunupyy");
    cityNames.insert(290, "Kuhmo");
    cityNames.insert(291, "Kuhmoinen");
    cityNames.insert(295, "Kumlinge");
    cityNames.insert(297, "Kuopio");
    cityNames.insert(300, "Kuortane");
    cityNames.insert(301, "Kurikka");
    cityNames.insert(304, "Kustavi");
    cityNames.insert(305, "Kuusamo");
    cityNames.insert(312, "Kyyjärvi");
    cityNames.insert(316, "Kärkölä");
    cityNames.insert(317, "Kärsämäki");
    cityNames.insert(318, "Kökar");
    cityNames.insert(319, "Köyliö");
    cityNames.insert(398, "Lahti");
    cityNames.insert(399, "Laihia");
    cityNames.insert(400, "Laitila");
    cityNames.insert(407, "Lapinjärvi");
    cityNames.insert(402, "Lapinlahti");
    cityNames.insert(403, "Lappajärvi");
    cityNames.insert(405, "Lappeenranta");
    cityNames.insert(408, "Lapua");
    cityNames.insert(410, "Laukaa");
    cityNames.insert(413, "Lavia");
    cityNames.insert(416, "Lemi");
    cityNames.insert(417, "Lemland");
    cityNames.insert(418, "Lempäälä");
    cityNames.insert(420, "Leppävirta");
    cityNames.insert(421, "Lestijärvi");
    cityNames.insert(422, "Lieksa");
    cityNames.insert(423, "Lieto");
    cityNames.insert(425, "Liminka");
    cityNames.insert(426, "Liperi");
    cityNames.insert(444, "Lohja");
    cityNames.insert(430, "Loimaa");
    cityNames.insert(433, "Loppi");
    cityNames.insert(434, "Loviisa");
    cityNames.insert(435, "Luhanka");
    cityNames.insert(436, "Lumijoki");
    cityNames.insert(438, "Lumparland");
    cityNames.insert(440, "Luoto");
    cityNames.insert(441, "Luumäki");
    cityNames.insert(442, "Luvia");
    cityNames.insert(475, "Maalahti");
    cityNames.insert(476, "Maaninka");
    cityNames.insert(478, "Maarianhamina - Mariehamn");
    cityNames.insert(480, "Marttila");
    cityNames.insert(481, "Masku");
    cityNames.insert(483, "Merijärvi");
    cityNames.insert(484, "Merikarvia");
    cityNames.insert(489, "Miehikkälä");
    cityNames.insert(491, "Mikkeli");
    cityNames.insert(494, "Muhos");
    cityNames.insert(495, "Multia");
    cityNames.insert(498, "Muonio");
    cityNames.insert(499, "Mustasaari");
    cityNames.insert(500, "Muurame");
    cityNames.insert(503, "Mynämäki");
    cityNames.insert(504, "Myrskylä");
    cityNames.insert(505, "Mäntsälä");
    cityNames.insert(508, "Mänttä-Vilppula");
    cityNames.insert(507, "Mäntyharju");
    cityNames.insert(529, "Naantali");
    cityNames.insert(531, "Nakkila");
    cityNames.insert(532, "Nastola");
    cityNames.insert(535, "Nivala");
    cityNames.insert(536, "Nokia");
    cityNames.insert(538, "Nousiainen");
    cityNames.insert(541, "Nurmes");
    cityNames.insert(543, "Nurmijärvi");
    cityNames.insert(545, "Närpiö");
    cityNames.insert(560, "Orimattila");
    cityNames.insert(561, "Oripää");
    cityNames.insert(562, "Orivesi");
    cityNames.insert(563, "Oulainen");
    cityNames.insert(564, "Oulu");
    cityNames.insert(309, "Outokumpu");
    cityNames.insert(576, "Padasjoki");
    cityNames.insert(577, "Paimio");
    cityNames.insert(578, "Paltamo");
    cityNames.insert(445, "Parainen");
    cityNames.insert(580, "Parikkala");
    cityNames.insert(581, "Parkano");
    cityNames.insert(599, "Pedersören kunta");
    cityNames.insert(583, "Pelkosenniemi");
    cityNames.insert(854, "Pello");
    cityNames.insert(584, "Perho");
    cityNames.insert(588, "Pertunmaa");
    cityNames.insert(592, "Petäjävesi");
    cityNames.insert(593, "Pieksämäki");
    cityNames.insert(595, "Pielavesi");
    cityNames.insert(598, "Pietarsaari");
    cityNames.insert(601, "Pihtipudas");
    cityNames.insert(604, "Pirkkala");
    cityNames.insert(607, "Polvijärvi");
    cityNames.insert(608, "Pomarkku");
    cityNames.insert(609, "Pori");
    cityNames.insert(611, "Pornainen");
    cityNames.insert(638, "Porvoo");
    cityNames.insert(614, "Posio");
    cityNames.insert(615, "Pudasjärvi");
    cityNames.insert(616, "Pukkila");
    cityNames.insert(619, "Punkalaidun");
    cityNames.insert(620, "Puolanka");
    cityNames.insert(623, "Puumala");
    cityNames.insert(624, "Pyhtää");
    cityNames.insert(625, "Pyhäjoki");
    cityNames.insert(626, "Pyhäjärvi");
    cityNames.insert(630, "Pyhäntä");
    cityNames.insert(631, "Pyhäranta");
    cityNames.insert(635, "Pälkäne");
    cityNames.insert(636, "Pöytyä");
    cityNames.insert(678, "Raahe");
    cityNames.insert(710, "Raasepori");
    cityNames.insert(680, "Raisio");
    cityNames.insert(681, "Rantasalmi");
    cityNames.insert(683, "Ranua");
    cityNames.insert(684, "Rauma");
    cityNames.insert(686, "Rautalampi");
    cityNames.insert(687, "Rautavaara");
    cityNames.insert(689, "Rautjärvi");
    cityNames.insert(691, "Reisjärvi");
    cityNames.insert(694, "Riihimäki");
    cityNames.insert(697, "Ristijärvi");
    cityNames.insert(698, "Rovaniemi");
    cityNames.insert(700, "Ruokolahti");
    cityNames.insert(702, "Ruovesi");
    cityNames.insert(704, "Rusko");
    cityNames.insert(707, "Rääkkylä");
    cityNames.insert(729, "Saarijärvi");
    cityNames.insert(732, "Salla");
    cityNames.insert(734, "Salo");
    cityNames.insert(736, "Saltvik");
    cityNames.insert(790, "Sastamala");
    cityNames.insert(738, "Sauvo");
    cityNames.insert(739, "Savitaipale");
    cityNames.insert(740, "Savonlinna");
    cityNames.insert(742, "Savukoski");
    cityNames.insert(743, "Seinäjoki");
    cityNames.insert(746, "Sievi");
    cityNames.insert(747, "Siikainen");
    cityNames.insert(748, "Siikajoki");
    cityNames.insert(791, "Siikalatva");
    cityNames.insert(749, "Siilinjärvi");
    cityNames.insert(751, "Simo");
    cityNames.insert(753, "Sipoo");
    cityNames.insert(755, "Siuntio");
    cityNames.insert(758, "Sodankylä");
    cityNames.insert(759, "Soini");
    cityNames.insert(761, "Somero");
    cityNames.insert(762, "Sonkajärvi");
    cityNames.insert(765, "Sotkamo");
    cityNames.insert(766, "Sottunga");
    cityNames.insert(768, "Sulkava");
    cityNames.insert(771, "Sund");
    cityNames.insert(777, "Suomussalmi");
    cityNames.insert(778, "Suonenjoki");
    cityNames.insert(781, "Sysmä");
    cityNames.insert(783, "Säkylä");
    cityNames.insert(831, "Taipalsaari");
    cityNames.insert(832, "Taivalkoski");
    cityNames.insert(833, "Taivassalo");
    cityNames.insert(834, "Tammela");
    cityNames.insert(837, "Tampere");
    cityNames.insert(838, "Tarvasjoki");
    cityNames.insert(844, "Tervo");
    cityNames.insert(845, "Tervola");
    cityNames.insert(846, "Teuva");
    cityNames.insert(848, "Tohmajärvi");
    cityNames.insert(849, "Toholampi");
    cityNames.insert(850, "Toivakka");
    cityNames.insert(851, "Tornio");
    cityNames.insert(853, "Turku");
    cityNames.insert(857, "Tuusniemi");
    cityNames.insert(858, "Tuusula");
    cityNames.insert(859, "Tyrnävä");
    cityNames.insert(886, "Ulvila");
    cityNames.insert(887, "Urjala");
    cityNames.insert(889, "Utajärvi");
    cityNames.insert(890, "Utsjoki");
    cityNames.insert(892, "Uurainen");
    cityNames.insert(893, "Uusikaarlepyy");
    cityNames.insert(895, "Uusikaupunki");
    cityNames.insert(785, "Vaala");
    cityNames.insert(905, "Vaasa");
    cityNames.insert(908, "Valkeakoski");
    cityNames.insert(911, "Valtimo");
    cityNames.insert(92, "Vantaa");
    cityNames.insert(915, "Varkaus");
    cityNames.insert(918, "Vehmaa");
    cityNames.insert(921, "Vesanto");
    cityNames.insert(922, "Vesilahti");
    cityNames.insert(924, "Veteli");
    cityNames.insert(925, "Vieremä");
    cityNames.insert(927, "Vihti");
    cityNames.insert(931, "Viitasaari");
    cityNames.insert(934, "Vimpeli");
    cityNames.insert(935, "Virolahti");
    cityNames.insert(936, "Virrat");
    cityNames.insert(941, "Vårdö");
    cityNames.insert(946, "Vöyri");
    cityNames.insert(976, "Ylitornio");
    cityNames.insert(977, "Ylivieska");
    cityNames.insert(980, "Ylöjärvi");
    cityNames.insert(981, "Ypäjä");
    cityNames.insert(989, "Ähtäri");
    cityNames.insert(992, "Äänekoski");

    finnishToSwedishCityNames.insert("Enontekiö", "Enontekis");
    finnishToSwedishCityNames.insert("Espoo", "Esbo");
    finnishToSwedishCityNames.insert("Eurajoki", "Euraåminne");
    finnishToSwedishCityNames.insert("Hailuoto", "Karlö");
    finnishToSwedishCityNames.insert("Hamina", "Fredrikshamn");
    finnishToSwedishCityNames.insert("Hanko", "Hangö");
    finnishToSwedishCityNames.insert("Helsinki", "Helsingfors");
    finnishToSwedishCityNames.insert("Hyvinkää", "Hyvinge");
    finnishToSwedishCityNames.insert("Hämeenkyrö", "Tavastkyro");
    finnishToSwedishCityNames.insert("Hämeenlinna", "Tavastehus");
    finnishToSwedishCityNames.insert("Iisalmi", "Idensalmi");
    finnishToSwedishCityNames.insert("Ikaalinen", "Ikalis");
    finnishToSwedishCityNames.insert("Ilomantsi", "Ilomants");
    finnishToSwedishCityNames.insert("Inari", "Enare");
    finnishToSwedishCityNames.insert("Inkoo", "Ingå");
    finnishToSwedishCityNames.insert("Isojoki", "Storå");
    finnishToSwedishCityNames.insert("Isokyrö", "Storkyro");
    finnishToSwedishCityNames.insert("Jokioinen", "Jockis");
    finnishToSwedishCityNames.insert("Joroinen", "Jorois");
    finnishToSwedishCityNames.insert("Järvenpää", "Träskända");
    finnishToSwedishCityNames.insert("Kaarina", "S:t Karins");
    finnishToSwedishCityNames.insert("Kajaani", "Kajana");
    finnishToSwedishCityNames.insert("Karijoki", "Bötom");
    finnishToSwedishCityNames.insert("Karkkila", "Högfors");
    finnishToSwedishCityNames.insert("Kaskinen", "Kaskö");
    finnishToSwedishCityNames.insert("Kauniainen", "Grankulla");
    finnishToSwedishCityNames.insert("Kaustinen", "Kaustby");
    finnishToSwedishCityNames.insert("Kemiönsaari", "Kimitoön");
    finnishToSwedishCityNames.insert("Kerava", "Kervo");
    finnishToSwedishCityNames.insert("Kirkkonummi", "Kyrkslätt");
    finnishToSwedishCityNames.insert("Kokemäki", "Kumo");
    finnishToSwedishCityNames.insert("Kokkola", "Karleby");
    finnishToSwedishCityNames.insert("Kristiinankaupunki", "Kristinestad");
    finnishToSwedishCityNames.insert("Kruunupyy", "Kronoby");
    finnishToSwedishCityNames.insert("Kustavi", "Gustavs");
    finnishToSwedishCityNames.insert("Köyliö", "Kjulo");
    finnishToSwedishCityNames.insert("Lahti", "Lahtis");
    finnishToSwedishCityNames.insert("Laihia", "Laihela");
    finnishToSwedishCityNames.insert("Lapinjärvi", "Lappträsk");
    finnishToSwedishCityNames.insert("Lappeenranta", "Villmanstrand");
    finnishToSwedishCityNames.insert("Lapua", "Lappo");
    finnishToSwedishCityNames.insert("Lieto", "Lundo");
    finnishToSwedishCityNames.insert("Liminka", "Limingo");
    finnishToSwedishCityNames.insert("Lohja", "Lojo");
    finnishToSwedishCityNames.insert("Loviisa", "Lovisa");
    finnishToSwedishCityNames.insert("Luoto", "Larsmo");
    finnishToSwedishCityNames.insert("Maalahti", "Malax");
    finnishToSwedishCityNames.insert("Maarianhamina - Mariehamn", "Mariehamn");
    finnishToSwedishCityNames.insert("Merikarvia", "Sastmola");
    finnishToSwedishCityNames.insert("Mikkeli", "S:t Michel");
    finnishToSwedishCityNames.insert("Mustasaari", "Korsholm");
    finnishToSwedishCityNames.insert("Myrskylä", "Mörskom");
    finnishToSwedishCityNames.insert("Naantali", "Nådendal");
    finnishToSwedishCityNames.insert("Nousiainen", "Nousis");
    finnishToSwedishCityNames.insert("Närpiö", "Närpes");
    finnishToSwedishCityNames.insert("Oulu", "Uleåborg");
    finnishToSwedishCityNames.insert("Paimio", "Pemar");
    finnishToSwedishCityNames.insert("Parainen", "Pargas");
    finnishToSwedishCityNames.insert("Pedersören kunta", "Pedersöre");
    finnishToSwedishCityNames.insert("Pietarsaari", "Jakobstad");
    finnishToSwedishCityNames.insert("Pirkkala", "Birkala");
    finnishToSwedishCityNames.insert("Pomarkku", "Påmark");
    finnishToSwedishCityNames.insert("Pori", "Björneborg");
    finnishToSwedishCityNames.insert("Pornainen", "Borgnäs");
    finnishToSwedishCityNames.insert("Porvoo", "Borgå");
    finnishToSwedishCityNames.insert("Pyhtää", "Pyttis");
    finnishToSwedishCityNames.insert("Raahe", "Brahestad");
    finnishToSwedishCityNames.insert("Raasepori", "Raseborg");
    finnishToSwedishCityNames.insert("Raisio", "Reso");
    finnishToSwedishCityNames.insert("Rauma", "Raumo");
    finnishToSwedishCityNames.insert("Sauvo", "Sagu");
    finnishToSwedishCityNames.insert("Savonlinna", "Nyslott");
    finnishToSwedishCityNames.insert("Sipoo", "Sibbo");
    finnishToSwedishCityNames.insert("Siuntio", "Sjundeå");
    finnishToSwedishCityNames.insert("Taivassalo", "Tövsala");
    finnishToSwedishCityNames.insert("Tampere", "Tammerfors");
    finnishToSwedishCityNames.insert("Teuva", "Östermark");
    finnishToSwedishCityNames.insert("Tornio", "Torneå");
    finnishToSwedishCityNames.insert("Turku", "Åbo");
    finnishToSwedishCityNames.insert("Tuusula", "Tusby");
    finnishToSwedishCityNames.insert("Ulvila", "Ulvsby");
    finnishToSwedishCityNames.insert("Uusikaarlepyy", "Nykarleby");
    finnishToSwedishCityNames.insert("Uusikaupunki", "Nystad");
    finnishToSwedishCityNames.insert("Vaasa", "Vasa");
    finnishToSwedishCityNames.insert("Vantaa", "Vanda");
    finnishToSwedishCityNames.insert("Veteli", "Vetil");
    finnishToSwedishCityNames.insert("Vihti", "Vichtis");
    finnishToSwedishCityNames.insert("Virrat", "Virdois");
    finnishToSwedishCityNames.insert("Vöyri", "Vörå");
    finnishToSwedishCityNames.insert("Ylitornio", "Övertorneå");
    finnishToSwedishCityNames.insert("Ähtäri", "Etseri");
}

bool ParserFinlandMatka::SortUtility::operator()(const Station& s1, const Station& s2)
{
    static const QStringList types_by_priority =
            QStringList() << STATION_TYPE_POI_DISTRICT
                          << STATION_TYPE_POI_TRANSPORTATION
                          << STATION_TYPE_STOP
                          << STATION_TYPE_POI_HOSPITAL
                          << STATION_TYPE_POI_ACCOMODATION
                          << STATION_TYPE_POI_SCHOOL
                          << STATION_TYPE_POI_MISC
                          << STATION_TYPE_POI_OTHER
                          << STATION_TYPE_STREET;
    if (s1.type == s2.type)
        return s1.name < s2.name;
    else {
        foreach (const QString& type, types_by_priority) {
            if (s1.type == type)
                return true;
            else if (s2.type == type)
                return false;
        }
        return s1.type < s2.type;
    }
}

bool ParserFinlandMatka::SortUtility::operator()(
        const SortableTimetableEntry& e1, const SortableTimetableEntry& e2)
{
    if (e1.dateTime != e2.dateTime)
        return e1.dateTime < e2.dateTime;
    else if (e1.entry.trainType != e2.entry.trainType)
        return e1.entry.trainType < e2.entry.trainType;
    else
        return e1.entry.destinationStation < e2.entry.destinationStation;
}
