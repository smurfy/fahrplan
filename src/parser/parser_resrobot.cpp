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

#include "parser_resrobot.h"

#include <QDebug>
#include <QNetworkReply>
#ifdef BUILD_FOR_QT5
    #include <QUrlQuery>
#endif

ParserResRobot::ParserResRobot(QObject *parent) :
        ParserAbstract(parent),
        timetableAPIKey(QLatin1String("en9A5GyxZLB98ZYjX8rkSNyHkurGb81G")),
        journeyAPIKey(QLatin1String("gcyYB9moXYXOTY2dAb06k7GAAOiZVXZr")),
        timetableBaseURL(QLatin1String("https://api.trafiklab.se/samtrafiken/resrobotstops/")),
        journeyBaseURL(QLatin1String("https://api.trafiklab.se/samtrafiken/resrobot/")),
        realtimeTimetableBaseURL(QLatin1String("https://api.trafiklab.se/samtrafiken/resrobotstopssuper/")),
        timetableAPIVersion(QLatin1String("2.2")),
        journeyAPIVersion(QLatin1String("2.1")),
        nearbyRadius(1000),
        timetableSpan(120),
        realtime(true)
{
    // Translate remarks
    remarkStrings[QString::fromUtf8("Bistrovagn")] = tr("Bistro car");
    remarkStrings[QString::fromUtf8("Djur ej tillåtet")] = tr("No animals");
    remarkStrings[QString::fromUtf8("Ej reservering")] = tr("No reservation");
    remarkStrings[QString::fromUtf8("Endast 2 klass")] = tr("Only second class");
    remarkStrings[QString::fromUtf8("Frivillig platsbokning")] = tr("Optional seat reservation");
    remarkStrings[QString::fromUtf8("Frukost 1 klass")] = tr("Breakfast in first class");
    remarkStrings[QString::fromUtf8("Internetuppkoppling")] = tr("Internet connection");
    remarkStrings[QString::fromUtf8("Mat 1 klass")] = tr("Food in first class");
    remarkStrings[QString::fromUtf8("Obligatorisk platsbokning")] = tr("Mandatory seat reservation");
    remarkStrings[QString::fromUtf8("Rullstolslyft")] = tr("Wheelchair lift");
    remarkStrings[QString::fromUtf8("Servering")] = tr("Catering");
    remarkStrings[QString::fromUtf8("Sov- och liggvagn")] = tr("Sleeping/dormitory car");

    // Translate transport modes
    transportModeStrings[QString::fromUtf8("Bil")] = tr("Car");
    transportModeStrings[QString::fromUtf8("Buss")] = tr("Bus");
    transportModeStrings[QString::fromUtf8("Ersättningsbuss")] = tr("Replacement bus");
    transportModeStrings[QString::fromUtf8("Ersättningstaxi")] = tr("Replacement taxi");
    transportModeStrings[QString::fromUtf8("Expressbuss")] = tr("Express bus");
    transportModeStrings[QString::fromUtf8("Flyg")] = "Flight";
    transportModeStrings[QString::fromUtf8("Flygbuss")] = tr("Airport bus");
    transportModeStrings[QString::fromUtf8("Flygbuss/flygtaxi")] = tr("Airport bus/airport taxi");
    transportModeStrings[QString::fromUtf8("Flygtaxi")] = tr("Airport taxi");
    transportModeStrings[QString::fromUtf8("Färja")] = tr("Ferry");
    transportModeStrings[QString::fromUtf8("Gång")] = tr("Walk");
    transportModeStrings[QString::fromUtf8("Gång lång sträcka")] = tr("Walk (long range)");
    transportModeStrings[QString::fromUtf8("Kajplats")] = tr("Berthage");
    transportModeStrings[QString::fromUtf8("Länstaxi")] = tr("County taxi");
    transportModeStrings[QString::fromUtf8("Nattåg")] = tr("Night train");
    transportModeStrings[QString::fromUtf8("Normalfärja lågpris")] = tr("Normal ferry (reduced price)");
    transportModeStrings[QString::fromUtf8("Pendeltåg")] = tr("Commuter train");
    transportModeStrings[QString::fromUtf8("Sittvagn i Nattåg")] = tr("Day coach in night train");
    transportModeStrings[QString::fromUtf8("Snabbtåg")] = tr("High-speed train");
    transportModeStrings[QString::fromUtf8("Snabbtåg anslutningstaxi")] = tr("High-speed train (taxi connection)");
    transportModeStrings[QString::fromUtf8("Specialbuss")] = tr("Special bus");
    transportModeStrings[QString::fromUtf8("Specialtåg")] = tr("Special train");
    transportModeStrings[QString::fromUtf8("Spårvagn")] = tr("Tram");
    transportModeStrings[QString::fromUtf8("Stadstrafik")] = tr("City traffic");
    transportModeStrings[QString::fromUtf8("Tunnelbana")] = tr("Metro");
    transportModeStrings[QString::fromUtf8("Tåg")] = tr("Train");
    transportModeStrings[QString::fromUtf8("Tågbuss")] = "Train bus";
    transportModeStrings[QString::fromUtf8("Tågtaxi")] = tr("Train taxi");
    transportModeStrings[QString::fromUtf8("Utrikes buss")] = "International bus";
    transportModeStrings[QString::fromUtf8("Utrikes färja")] = tr("International ferry");
    transportModeStrings[QString::fromUtf8("Utrikes tåg")] = tr("International train");
    transportModeStrings[QString::fromUtf8("Övriga tåg")] = tr("Other train");
}

ParserResRobot::~ParserResRobot()
{
    clearJourney();
}

void ParserResRobot::clearJourney()
{
    if (lastJourneyResultList) {
        delete lastJourneyResultList;
        lastJourneyResultList = NULL;
    }
}

bool ParserResRobot::supportsGps()
{
    return true;
}

bool ParserResRobot::supportsVia()
{
    return false;
}

bool ParserResRobot::supportsTimeTable()
{
    return true;
}

bool ParserResRobot::supportsTimeTableDirection()
{
    return false;
}

QStringList ParserResRobot::getTrainRestrictions()
{
    QStringList list;
    list << tr("All")
         << tr("Train and local trans., not express bus")
         << tr("Express bus and local trans., not train");
    return list;
}

void ParserResRobot::findStationsByName(const QString &stationName)
{
    lastStationSearch = stationName;
    if (stationName.length() < 2)
        return;
    if (currentRequestState != FahrplanNS::noneRequest)
        return;
    currentRequestState = FahrplanNS::stationsByNameRequest;

    QUrl url(journeyBaseURL + QLatin1String("FindLocation.json"));
#if defined(BUILD_FOR_QT5)
    QUrlQuery query;
#else
    QUrl query;
#endif
    query.addQueryItem("key", journeyAPIKey);
    query.addQueryItem("apiVersion", journeyAPIVersion);
    query.addQueryItem("from", stationName);
    query.addQueryItem("coordSys", "WGS84");
#if defined(BUILD_FOR_QT5)
    url.setQuery(query);
#else
    url.setQueryItems(query.queryItems());
#endif
    sendHttpRequest(url);
}

void ParserResRobot::findStationsByCoordinates(qreal longitude, qreal latitude)
{
    if (currentRequestState != FahrplanNS::noneRequest)
        return;
    currentRequestState = FahrplanNS::stationsByCoordinatesRequest;

    QUrl url(journeyBaseURL + QLatin1String("StationsInZone.json"));
#if defined(BUILD_FOR_QT5)
    QUrlQuery query;
#else
    QUrl query;
#endif
    query.addQueryItem("key", journeyAPIKey);
    query.addQueryItem("apiVersion", journeyAPIVersion);
    query.addQueryItem("centerX", QString::number(longitude));
    query.addQueryItem("centerY", QString::number(latitude));
    query.addQueryItem("radius", QString::number(nearbyRadius));
    query.addQueryItem("coordSys", "WGS84");
#if defined(BUILD_FOR_QT5)
    url.setQuery(query);
#else
    url.setQueryItems(query.queryItems());
#endif
    sendHttpRequest(url);
}

void ParserResRobot::getTimeTableForStation(const Station &currentStation,
                                            const Station &directionStation,
                                            const QDateTime &datetime,
                                            ParserAbstract::Mode mode,
                                            int trainrestrictions)
{
    Q_UNUSED(directionStation)
    Q_UNUSED(datetime)
    Q_UNUSED(mode)
    Q_UNUSED(trainrestrictions)

    if (currentRequestState != FahrplanNS::noneRequest)
        return;
    currentRequestState = FahrplanNS::getTimeTableForStationRequest;

    QUrl url;
    if (realtime)
        url = realtimeTimetableBaseURL + QLatin1String("GetDepartures.json");
    else
        url = timetableBaseURL + QLatin1String("GetDepartures.json");
#if defined(BUILD_FOR_QT5)
    QUrlQuery query;
#else
    QUrl query;
#endif
    query.addQueryItem("key", timetableAPIKey);
    query.addQueryItem("apiVersion", timetableAPIVersion);
    query.addQueryItem("locationId", currentStation.id.toString());
    query.addQueryItem("timeSpan", QString::number(timetableSpan));
    query.addQueryItem("coordSys", "WGS84");
#if defined(BUILD_FOR_QT5)
    url.setQuery(query);
#else
    url.setQueryItems(query.queryItems());
#endif
    sendHttpRequest(url);
}

void ParserResRobot::searchJourney(const Station &departureStation, const Station &viaStation,
                                   const Station &arrivalStation, const QDateTime &dateTime,
                                   ParserAbstract::Mode mode, int trainRestrictions)
{
    if (currentRequestState != FahrplanNS::noneRequest)
        return;
    currentRequestState = FahrplanNS::searchJourneyRequest;
    numberOfUnsuccessfulEarlierSearches = 0;
    numberOfUnsuccessfulLaterSearches = 0;
    internalSearchJourney(departureStation, viaStation, arrivalStation, dateTime, mode, trainRestrictions);
}

void ParserResRobot::searchJourneyLater()
{
    // If the last "later" search didn't give any new results, try searching one
    // hour later than last time, otherwise search on the time of the last result option.
    QDateTime time;
    if (lastJourneySearch.mode == Departure)
        time = lastJourneySearch.lastOption.addSecs(numberOfUnsuccessfulLaterSearches * 3600);
    else
        time = lastJourneySearch.lastOption.addSecs(numberOfUnsuccessfulLaterSearches * 3600 + 3600);
    currentRequestState = FahrplanNS::searchJourneyLaterRequest;
    internalSearchJourney(lastJourneySearch.from, lastJourneySearch.via, lastJourneySearch.to,
                          time, lastJourneySearch.mode, lastJourneySearch.restrictions);
}

void ParserResRobot::searchJourneyEarlier()
{
    // If last the "earlier" search didn't give any new results, try searching one
    // hour earlier than last time, otherwise search on the time of the first result
    // option minus one hour.
    QDateTime time;
    if (lastJourneySearch.mode == Departure)
        time = lastJourneySearch.firstOption.addSecs(numberOfUnsuccessfulEarlierSearches * -3600 - 3600);
    else
        time = lastJourneySearch.firstOption.addSecs(numberOfUnsuccessfulEarlierSearches * -3600);
    currentRequestState = FahrplanNS::searchJourneyEarlierRequest;
    internalSearchJourney(lastJourneySearch.from, lastJourneySearch.via, lastJourneySearch.to,
                          time, lastJourneySearch.mode, lastJourneySearch.restrictions);
}

void ParserResRobot::internalSearchJourney(const Station &departureStation, const Station &viaStation,
                                           const Station &arrivalStation, const QDateTime &dateTime,
                                           ParserAbstract::Mode mode, int trainRestrictions)
{
    Q_UNUSED(viaStation)

    lastJourneySearch.dateTime = dateTime;
    lastJourneySearch.from = departureStation;
    lastJourneySearch.via = viaStation;
    lastJourneySearch.to = arrivalStation;
    lastJourneySearch.restrictions = trainRestrictions;
    lastJourneySearch.mode = mode;

    QUrl url = journeyBaseURL + QLatin1String("Search.json");
#if defined(BUILD_FOR_QT5)
    QUrlQuery query;
#else
    QUrl query;
#endif
    query.addQueryItem("key", journeyAPIKey);
    query.addQueryItem("apiVersion", journeyAPIVersion);
    query.addQueryItem("fromId", departureStation.id.toString());
    query.addQueryItem("toId", arrivalStation.id.toString());
    query.addQueryItem("date", dateTime.toString("yyyy-MM-dd"));
    query.addQueryItem("time", dateTime.toString("hh:mm"));
    query.addQueryItem("coordSys", "WGS84");
    if (mode == Arrival)
        query.addQueryItem("arrival", "true");
    QString transportModeCode;
    switch (trainRestrictions) {
    default:
    case ALL_TRANSPORT_MODES:
        transportModeCode = "F";
        break;
    case TRAIN_PUB_TRANS_NOT_EXP_BUS:
        transportModeCode = "T";
        break;
    case EXP_BUS_PUB_TRANS_NOT_TRAIN:
        transportModeCode = "B";
        break;
    }
    query.addQueryItem("searchType", transportModeCode);

#if defined(BUILD_FOR_QT5)
    url.setQuery(query);
#else
    url.setQueryItems(query.queryItems());
#endif
    sendHttpRequest(url);
}

void ParserResRobot::parseTimeTable(QNetworkReply *networkReply)
{
    QByteArray allData = networkReply->readAll();
    //qDebug() << "Reply:\n" << allData;

    QVariantMap doc = parseJson(allData);
    if (doc.isEmpty()) {
        emit errorOccured(tr("Cannot parse reply from the server"));
        return;
    }

    QVariantMap departuresResult= doc.value("getdeparturesresult").toMap();
    QVariantList timetableData;
    if (departuresResult.contains("departuresegment"))
        timetableData = ensureList(departuresResult.value("departuresegment"));

    TimetableEntriesList timetable;
    foreach (QVariant timetableEntryData, timetableData) {
        QVariantMap entry = timetableEntryData.toMap();
        TimetableEntry resultItem;

        QVariantMap departure = entry.value("departure").toMap();
        resultItem.currentStation = departure.value("location").toMap().value("name").toString();
        resultItem.longitude = departure.value("@x").toDouble();
        resultItem.latitude = departure.value("@y").toDouble();
        resultItem.platform = departure.value("stoppoint").toString();
        resultItem.time = QDateTime::fromString(departure.value("datetime").toString(), "yyyy-MM-dd HH:mm").time();

        // Destination
        resultItem.destinationStation = entry.value("direction").toString();

        // Realtime info
        QVariantMap realtimeInfo = entry.value("realtime").toMap();
        bool hasDeviationInfo;
        int deviation = realtimeInfo.value("departuretimedeviation").toInt(&hasDeviationInfo);
        if (hasDeviationInfo && deviation != 0)
            resultItem.miscInfo = tr("New time: ") + resultItem.time.addSecs(deviation * 60).toString("HH:mm");

        // Means of transportation
        QVariantMap mot = entry.value("segmentid").toMap().value("mot").toMap();
        resultItem.trainType = translateTransportMode(mot.value("#text").toString());
        QVariantMap carrier = entry.value("segmentid").toMap().value("carrier").toMap();
        if (carrier.size() > 0) {
            QString carrierNumber = carrier.value("number").toString();
            if (!carrierNumber.isEmpty())
                resultItem.trainType += " " + carrierNumber;
        }

        timetable.append(resultItem);
    }
    emit timetableResult(timetable);
}

void ParserResRobot::parseStationsByName(QNetworkReply *networkReply)
{
    QByteArray allData = networkReply->readAll();
    //qDebug() << "Reply:\n" << allData;

    QVariantMap doc = parseJson(allData);
    if (doc.isEmpty()) {
        emit errorOccured(tr("Cannot parse reply from the server"));
        return;
    }

    QVariantList stations = ensureList(doc.value("findlocationresult").toMap()
                                       .value("from").toMap().value("location"));

    StationsList result;
    foreach (QVariant stationData, stations) {
        QVariantMap station = stationData.toMap();
        Station s;
        s.id = station.value("locationid").toString();
        s.name = station.value("displayname").toString();
        s.longitude = station.value("@x").toDouble();
        s.latitude = station.value("@y").toDouble();

        // Extra filter to get rid of nonsense in results
        // Example: Searching for "Wieselgren" returns stuff like
        //          "Lagersbergsparken (Eskilstuna kn)" which probably isn't what the user meant
        if (s.name.contains(lastStationSearch, Qt::CaseInsensitive))
            result.append(s);
    }

    emit stationsResult(result);
}

void ParserResRobot::parseStationsByCoordinates(QNetworkReply *networkReply)
{
    QByteArray allData = networkReply->readAll();
    //qDebug() << "Reply:\n" << allData;

    QVariantMap doc = parseJson(allData);
    if (doc.isEmpty()) {
        emit errorOccured(tr("Cannot parse reply from the server"));
        return;
    }

    QVariantList stations = ensureList(doc.value("stationsinzoneresult").toMap().value("location"));

    StationsList result;
    foreach (QVariant stationData, stations) {
        QVariantMap station = stationData.toMap();
        Station s;
        s.id = station.value("@id").toString();
        s.name = station.value("name").toString();
        s.longitude = station.value("@x").toDouble();
        s.latitude = station.value("@y").toDouble();
        result.append(s);
    }

    emit stationsResult(result);
}

void ParserResRobot::parseSearchJourney(QNetworkReply *networkReply)
{
    QByteArray allData = networkReply->readAll();
    //qDebug() << "Reply:\n" << allData;

    QVariantMap doc = parseJson(allData);
    if (doc.isEmpty()) {
        emit errorOccured(tr("Cannot parse reply from the server"));
        return;
    }

    QVariantMap timetableResult = doc.value("timetableresult").toMap();
    QVariantList journeyListData;
    if (timetableResult.contains("ttitem"))
        journeyListData = ensureList(timetableResult.value("ttitem"));

    cachedResults.clear();

    clearJourney();
    lastJourneyResultList = new JourneyResultList(this);

    int journeyCounter = 0;
    foreach (QVariant journeyData, journeyListData) {
        QString journeyID = QString::number(journeyCounter);
        QList<JourneyDetailResultItem*> segments = parseJourneySegments(journeyData.toMap());
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
        if (transportModes.count() == 0 && segments.count() == 1)
            transportModes.append(segments.first()->train());

        JourneyDetailResultList* journeyDetails = new JourneyDetailResultList(this);
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

        JourneyResultItem* journey = new JourneyResultItem(lastJourneyResultList);
        journey->setId(journeyID);
        journey->setDate(segments.first()->departureDateTime().date());
        journey->setDepartureTime(depTime);
        journey->setArrivalTime(arrTime);
        journey->setTrainType(transportModes.join(", "));
        journey->setDuration(duration);
        journey->setTransfers(QString::number(transportModes.count()-1));
        lastJourneyResultList->appendItem(journey);

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
    lastJourneyResultList->setDepartureStation(lastJourneySearch.from.name);
    lastJourneyResultList->setArrivalStation(lastJourneySearch.to.name);
    QString modeString;
    if (lastJourneySearch.mode == Arrival)
        modeString = tr("Arrivals");
    else
        modeString = tr("Departures");
    lastJourneyResultList->setTimeInfo(modeString + " " + lastJourneySearch.dateTime.toString(tr("ddd MMM d, HH:mm")));

    emit journeyResult(lastJourneyResultList);
}

// Parse info about one journey option. Store detailed info about segments for later use.
QList<JourneyDetailResultItem*> ParserResRobot::parseJourneySegments(const QVariantMap &journeyData)
{
    QList<JourneyDetailResultItem*> results;

    QVariantList segments = ensureList(journeyData.value("segment"));
    foreach (QVariant segmentData, segments)
    {
        QVariantMap segment = segmentData.toMap();
        JourneyDetailResultItem* resultItem = new JourneyDetailResultItem;//FIXME will leak in QML

        // Departure
        QVariantMap departure = segment.value("departure").toMap();
        resultItem->setDepartureStation(departure.value("location").toMap().value("name").toString());
        resultItem->setDepartureDateTime(QDateTime::fromString(departure.value("datetime").toString(), "yyyy-MM-dd HH:mm"));

        // Arrival
        QVariantMap arrival = segment.value("arrival").toMap();
        resultItem->setArrivalStation(arrival.value("location").toMap().value("name").toString());
        resultItem->setArrivalDateTime(QDateTime::fromString(arrival.value("datetime").toString(), "yyyy-MM-dd HH:mm"));


        QStringList info;

        // Remarks
        if (segment.contains("remarks")) {
            QVariantList remarks = ensureList(segment.value("remarks").toMap().value("remark"));
            foreach (QVariant remark, remarks)
                info.append(translateRemark(remark.toMap().value("#text").toString()));
        }

        // Means of transportation
        QVariantMap mot = segment.value("segmentid").toMap().value("mot").toMap();
        QString type = mot.value("@type").toString();
        QString motName = translateTransportMode(mot.value("#text").toString());
        QString distance;
        if (type == "G" || type == "GL") { // Walk or long walk
            distance = segment.value("segmentid").toMap().value("distance").toString();
            resultItem->setInternalData1("WALK");
        }
        QVariantMap carrier = segment.value("segmentid").toMap().value("carrier").toMap();
        QString carrierInfo;
        if (carrier.size() > 0) {
            QString carrierNumber = carrier.value("number").toString();
            if (!carrierNumber.isEmpty())
                motName += " " + carrierNumber;
            QString carrierName = carrier.value("name").toString();
            QString carrierURL = carrier.value("url").toString();
            if (!carrierName.isEmpty()) {
                if (carrierURL.isEmpty())
                    carrierInfo = carrierName;
                else
                    carrierInfo = "<a href=\"" + carrierURL + "\">" + carrierName + "</a>";
            }
        }
        resultItem->setTrain(motName);

        if (!distance.isEmpty())
            resultItem->setInfo(distance + " m");
        else if (!carrierInfo.isEmpty() && !info.isEmpty())
            resultItem->setInfo(carrierInfo + "<br/>" + info.join(", "));
        else if (!carrierInfo.isEmpty())
            resultItem->setInfo(carrierInfo);
        else if (!info.isEmpty())
            resultItem->setInfo(info.join(", "));
        resultItem->setDirection(segment.value("direction").toString());

        results.append(resultItem);
    }
    return results;
}

void ParserResRobot::getJourneyDetails(const QString &id)
{
    if (cachedResults.contains(id))
        emit journeyDetailsResult(cachedResults.value(id));
}

void ParserResRobot::parseSearchLaterJourney(QNetworkReply *networkReply)
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

void ParserResRobot::parseSearchEarlierJourney(QNetworkReply *networkReply)
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

void ParserResRobot::parseJourneyDetails(QNetworkReply *networkReply)
{
    Q_UNUSED(networkReply)
    // Should never happen since we get all info about all journeys at once
    // and therefore never need to send any journeyDetailsRequest.
}

// If a list only contains one item, the API skips the list and sets the list variable
// to the item directly. This function ensures that the variable always is a list.
QVariantList ParserResRobot::ensureList(const QVariant &variable)
{
    if (variable.type() == QVariant::List) {
        return variable.toList();
    } else {
        QVariantList list;
        list.append(variable);
        return list;
    }
}

QString ParserResRobot::translateRemark(const QString& original)
{
    if (remarkStrings.contains(original)) {
        return remarkStrings[original];
    } else {
        //qDebug() << "Didn't find translation of" << original;
        return original;
    }
}

QString ParserResRobot::translateTransportMode(QString original)
{
    original.replace(QString::fromUtf8("Ersättningsbuss"), tr("Replacement bus"));
    original.replace(QString::fromUtf8("Ersättningstaxi"), tr("Replacement taxi"));
    if (transportModeStrings.contains(original)) {
        return transportModeStrings[original];
    } else {
        //qDebug() << "Didn't find translation of" << original;
        return original;
    }
}
