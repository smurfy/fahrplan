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

ParserResRobot::ParserResRobot(QObject *parent) :
        ParserAbstract(parent),
        timetableAPIKey(QLatin1String("75d0c2b5-c179-489c-90c2-eb6d2bc8970c")),
        journeyAPIKey(QLatin1String("c8436ea6-3c7e-489f-93b1-5b636fc55f2e")),
        baseURL(QLatin1String("https://api.resrobot.se/v2"))
{
    // Translate remarks
    hafasAttributes.insert("A1", tr("Food in first class"));
    hafasAttributes.insert("A6", tr("Business class Plus available"));
    hafasAttributes.insert("A7", tr("No pets allowed"));
    hafasAttributes.insert("AA", tr("Standard class only"));
    hafasAttributes.insert("AB", tr("Seat reservation not possible in second class"));
    hafasAttributes.insert("AC", tr("Trolley service"));
    hafasAttributes.insert("AD", tr("Restaurant"));
    hafasAttributes.insert("AE", tr("No reservation"));
    hafasAttributes.insert("AF", tr("Mandatory seat reservation"));
    hafasAttributes.insert("AG", tr("Optional seat reservation"));
    hafasAttributes.insert("AH", tr("Access to sleeper before dep."));
    hafasAttributes.insert("AI", tr("Access to sleeper after arr."));
    hafasAttributes.insert("AJ", tr("Telephone reservation"));
    hafasAttributes.insert("AK", tr("Family coach"));
    hafasAttributes.insert("AL", tr("Coach with cinema and bistro"));
    hafasAttributes.insert("AM", tr("Supplement"));
    hafasAttributes.insert("AN", tr("Wheelchair lift"));
    hafasAttributes.insert("AO", tr("Short distance trips not allowed"));
    hafasAttributes.insert("AP", tr("Regional fare for reg. journey"));
    hafasAttributes.insert("AQ", tr("Engineering work. Bus/taxi repl."));
    hafasAttributes.insert("AR", tr("Detour due to engineering work"));
    hafasAttributes.insert("AS", tr("Engineering work. 60-90 min delay."));
    hafasAttributes.insert("AT", tr("Reduced speed (may be delayed)"));
    hafasAttributes.insert("AU", tr("Flexicoach with bistro"));
    hafasAttributes.insert("AV", tr("Bed and wheelchair or bed + parent with child"));
    hafasAttributes.insert("AW", tr("Internet connection"));
    hafasAttributes.insert("AZ", tr("Preordered breakfast"));
    hafasAttributes.insert("BF", tr("No reservation in first class"));
    hafasAttributes.insert("BG", tr("Luggage"));
    hafasAttributes.insert("D1", tr("Animals allowed, see rules"));
    hafasAttributes.insert("D2", tr("Animals allowed, see rules"));
    hafasAttributes.insert("D3", tr("Animals allowed, see rules"));
    hafasAttributes.insert("EL", tr("Electricity for reservation"));
    hafasAttributes.insert("ES", tr("No day coach"));
    hafasAttributes.insert("FB", tr("Carriage of bicycle"));
    hafasAttributes.insert("FM", tr("First class without food has no reservation"));
    hafasAttributes.insert("KI", tr("Skis"));
    hafasAttributes.insert("KO", tr("Office coach"));
    hafasAttributes.insert("KU", tr("Culture coach"));
    hafasAttributes.insert("LW", tr("Couchette"));
    hafasAttributes.insert("PE", tr("Newspapers in first class"));
    hafasAttributes.insert("PS", tr("Society coach"));
    hafasAttributes.insert("S4", tr("Four-bed compartment in sleeper"));
    hafasAttributes.insert("SD", tr("Alcohol service"));
    hafasAttributes.insert("SF", tr("Breakfast in first class"));
    hafasAttributes.insert("SG", tr("Tavern"));
    hafasAttributes.insert("SH", tr("Reduced speed, may be delayed"));
    hafasAttributes.insert("SI", tr("No foodservice"));
    hafasAttributes.insert("SK", tr("Café"));
    hafasAttributes.insert("SL", tr("Sleeper and couchette"));
    hafasAttributes.insert("SM", tr("Foodservice"));
    hafasAttributes.insert("SN", tr("Foodservice plus"));
    hafasAttributes.insert("SS", tr("Food served at seat in first class"));
    hafasAttributes.insert("SV", tr("Hot food may be preordered"));
    hafasAttributes.insert("SW", tr("Sleeper"));
    hafasAttributes.insert("00", tr("Faster service (overtakes slower services)"));
    hafasAttributes.insert("P-", tr("No price information available"));

    specificTransportModes.insert("BAX", tr("Airport transfer (bus)"));
    specificTransportModes.insert("BOR", QString::fromUtf8("Öresundståg (%1)").arg(tr("replacement bus")));
    specificTransportModes.insert("BPT", QString::fromUtf8("Pågatåg (%1)").arg(tr("replacement bus")));
    specificTransportModes.insert("BXB", tr("Express bus"));
    specificTransportModes.insert("FNF", tr("Normal ferry (reduced price)"));
    specificTransportModes.insert("FUT", tr("International ferry"));
    specificTransportModes.insert("FXF", QString("%1 SF700 %2").arg(tr("Express ferry")).arg(tr("yellow dep")));
    specificTransportModes.insert("FXL", QString("%1 SF1500 %2").arg(tr("Express ferry")).arg(tr("white dep")));
    specificTransportModes.insert("FXM", QString("%1 SF700 %2").arg(tr("Express ferry")).arg(tr("white dep")));
    specificTransportModes.insert("FXN", QString("%1 SF1500 %2").arg(tr("Express ferry")).arg(tr("yellow dep")));
    specificTransportModes.insert("JAV", "Renfe Ave");
    specificTransportModes.insert("JAX", tr("Airport transfer (train)"));
    specificTransportModes.insert("JCN", "City Night Line");
    specificTransportModes.insert("JCS", tr("Night train"));
    specificTransportModes.insert("JEC", "EuroCity");
    specificTransportModes.insert("JEN", "EuroNight");
    specificTransportModes.insert("JES", "Eurostar Italia");
    specificTransportModes.insert("JEX", tr("Express train"));
    specificTransportModes.insert("JFM", QString::fromUtf8("Flåmsbanen"));
    specificTransportModes.insert("JGM", "Flytog");
    specificTransportModes.insert("JIC", "InterCity");
    specificTransportModes.insert("JIE", "ICE");
    specificTransportModes.insert("JIL", "InterCity Lyn");
    specificTransportModes.insert("JIR", "InterRegio");
    specificTransportModes.insert("JKP", "Kustpilen");
    specificTransportModes.insert("JNO", QString::fromUtf8("Norrtåg"));
    specificTransportModes.insert("JNS", "NSB/SJ");
    specificTransportModes.insert("JNT", tr("Night train"));
    specificTransportModes.insert("JNZ", "DB Nachtzug");
    specificTransportModes.insert("JOR", QString::fromUtf8("Öresundståg"));
    specificTransportModes.insert("JPT", QString::fromUtf8("Pågatåg"));
    specificTransportModes.insert("JSP", tr("Special train"));
    specificTransportModes.insert("JST", tr("High-speed train"));
    specificTransportModes.insert("JTH", "Thalys");
    specificTransportModes.insert("TAX", tr("Airport transfer (taxi)"));

    generalTransportModes.insert("A", tr("Flight"));
    generalTransportModes.insert("B", tr("Bus"));
    generalTransportModes.insert("F", tr("Ferry"));
    generalTransportModes.insert("H", tr("Helicopter"));
    generalTransportModes.insert("J", tr("Train"));
    generalTransportModes.insert("S", tr("Tram"));
    generalTransportModes.insert("T", tr("Taxi"));
    generalTransportModes.insert("U", tr("Rapid transit"));
}

bool ParserResRobot::supportsGps()
{
    return true;
}

bool ParserResRobot::supportsVia()
{
    return true;
}

bool ParserResRobot::supportsTimeTable()
{
    return true;
}

bool ParserResRobot::supportsTimeTableDirection()
{
    return true;
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

    QUrl url(baseURL + QLatin1String("/location.name"));
#if defined(BUILD_FOR_QT5)
    QUrlQuery query;
#else
    QUrl query;
#endif
    query.addQueryItem("key", journeyAPIKey);
    query.addQueryItem("input", stationName);
    query.addQueryItem("maxNo", "30"); // Max number of results
    if (QLocale().language() == QLocale::Swedish)
        query.addQueryItem("lang", "sv");
    else if (QLocale().language() == QLocale::German)
        query.addQueryItem("lang", "de");
    else
        query.addQueryItem("lang", "en");
    query.addQueryItem("format", "json");
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

    QUrl url(baseURL + QLatin1String("/location.nearbystops"));
#if defined(BUILD_FOR_QT5)
    QUrlQuery query;
#else
    QUrl query;
#endif
    query.addQueryItem("key", journeyAPIKey);
    query.addQueryItem("originCoordLat", QString::number(latitude));
    query.addQueryItem("originCoordLong", QString::number(longitude));
    query.addQueryItem("r", "1000"); // Radius in meters
    query.addQueryItem("maxNo", "30"); // Max number of results
    if (QLocale().language() == QLocale::Swedish)
        query.addQueryItem("lang", "sv");
    else if (QLocale().language() == QLocale::German)
        query.addQueryItem("lang", "de");
    else
        query.addQueryItem("lang", "en");
    query.addQueryItem("format", "json");
#if defined(BUILD_FOR_QT5)
    url.setQuery(query);
#else
    url.setQueryItems(query.queryItems());
#endif
    sendHttpRequest(url);
}

void ParserResRobot::getTimeTableForStation(const Station &currentStation,
                                            const Station &directionStation,
                                            const QDateTime &dateTime,
                                            ParserAbstract::Mode mode,
                                            int trainrestrictions)
{
    if (currentRequestState != FahrplanNS::noneRequest)
        return;
    currentRequestState = FahrplanNS::getTimeTableForStationRequest;

    timetableSearchMode = mode;

    QUrl url;
    if (mode == Arrival)
        url.setUrl(baseURL + QLatin1String("/arrivalBoard"));
    else
        url.setUrl(baseURL + QLatin1String("/departureBoard"));
#if defined(BUILD_FOR_QT5)
    QUrlQuery query;
#else
    QUrl query;
#endif
    query.addQueryItem("key", timetableAPIKey);
    query.addQueryItem("id", stationIDv2(currentStation.id.toString()));
    if (directionStation.valid)
        query.addQueryItem("direction", stationIDv2(directionStation.id.toString()));
    query.addQueryItem("date", dateTime.toString("yyyy-MM-dd"));
    query.addQueryItem("time", dateTime.toString("hh:mm"));
    query.addQueryItem("maxJourneys", "30"); // Max number of results
    query.addQueryItem("passlist", "0"); // We don't need any intermediate stops in the result
    QString formattedRestrictions(formatRestrictions(trainrestrictions));
    if (!formattedRestrictions.isEmpty())
        query.addQueryItem("products", formattedRestrictions);
    if (QLocale().language() == QLocale::Swedish)
        query.addQueryItem("lang", "sv");
    else if (QLocale().language() == QLocale::German)
        query.addQueryItem("lang", "de");
    else
        query.addQueryItem("lang", "en");
    query.addQueryItem("format", "json");

#if defined(BUILD_FOR_QT5)
    url.setQuery(query);
#else
    url.setQueryItems(query.queryItems());
#endif
    qDebug() << "Searching for timetable:" << url.toString();
    sendHttpRequest(url);
}

void ParserResRobot::searchJourney(const Station &departureStation, const Station &viaStation,
                                   const Station &arrivalStation, const QDateTime &dateTime,
                                   ParserAbstract::Mode mode, int trainRestrictions)
{
    if (currentRequestState != FahrplanNS::noneRequest)
        return;
    currentRequestState = FahrplanNS::searchJourneyRequest;
    lastJourneySearch.dateTime = dateTime;
    lastJourneySearch.from = departureStation;
    lastJourneySearch.via = viaStation;
    lastJourneySearch.to = arrivalStation;
    lastJourneySearch.restrictions = trainRestrictions;
    lastJourneySearch.mode = mode;

    lastJourneySearch.from.id = stationIDv2(lastJourneySearch.from.id.toString());
    lastJourneySearch.via.id = stationIDv2(lastJourneySearch.via.id.toString());
    lastJourneySearch.to.id = stationIDv2(lastJourneySearch.to.id.toString());

#if defined(BUILD_FOR_QT5)
    QUrlQuery query;
#else
    QUrl query;
#endif

    query.addQueryItem("date", dateTime.toString("yyyy-MM-dd"));
    query.addQueryItem("time", dateTime.toString("hh:mm"));

    doSearchJourney(query);
}

void ParserResRobot::searchJourneyLater()
{
    currentRequestState = FahrplanNS::searchJourneyLaterRequest;
#if defined(BUILD_FOR_QT5)
    QUrlQuery query;
#else
    QUrl query;
#endif
    query.addQueryItem("context", searchLaterReference);
    doSearchJourney(query);
}

void ParserResRobot::searchJourneyEarlier()
{
    currentRequestState = FahrplanNS::searchJourneyEarlierRequest;
#if defined(BUILD_FOR_QT5)
    QUrlQuery query;
#else
    QUrl query;
#endif
    query.addQueryItem("context", searchEarlierReference);
    doSearchJourney(query);
}

#if defined(BUILD_FOR_QT5)
void ParserResRobot::doSearchJourney(QUrlQuery query)
#else
void ParserResRobot::doSearchJourney(QUrl query)
#endif
{
    QUrl url = baseURL + QLatin1String("/trip");
    if (lastJourneySearch.mode == Arrival)
        query.addQueryItem("searchForArrival", "1");
    query.addQueryItem("key", journeyAPIKey);
    query.addQueryItem("originId", lastJourneySearch.from.id.toString());
    if (lastJourneySearch.via.valid)
        query.addQueryItem("viaId", lastJourneySearch.via.id.toString());
    query.addQueryItem("destId", lastJourneySearch.to.id.toString());
    query.addQueryItem("passlist", "0"); // We don't need any intermediate stops in the result
    query.addQueryItem("format", "json");
    QString formattedRestrictions(formatRestrictions(lastJourneySearch.restrictions));
    if (!formattedRestrictions.isEmpty())
        query.addQueryItem("products", formattedRestrictions);
    if (QLocale().language() == QLocale::Swedish)
        query.addQueryItem("lang", "sv");
    else if (QLocale().language() == QLocale::German)
        query.addQueryItem("lang", "de");
    else
        query.addQueryItem("lang", "en");

#if defined(BUILD_FOR_QT5)
    url.setQuery(query);
#else
    url.setQueryItems(query.queryItems());
#endif
    qDebug() << "Searching for journey:" << url.toString();
    sendHttpRequest(url);
}

void ParserResRobot::parseTimeTable(QNetworkReply *networkReply)
{
    QByteArray allData = networkReply->readAll();
    qDebug() << "Reply:\n" << allData;

    QVariantMap doc = parseJson(allData);
    if (doc.isEmpty()) {
        emit errorOccured(tr("Cannot parse reply from the server"));
        return;
    }

    QVariantList departures;
    if (timetableSearchMode == Arrival)
        departures = doc.value("Arrival").toList();
    else
        departures = doc.value("Departure").toList();
    TimetableEntriesList timetable;
    foreach (QVariant departureData, departures) {
        TimetableEntry resultItem;
        const QVariantMap& departure = departureData.toMap();
        const QVariantMap& product = departure.value("Product").toMap();
        QStringList info;

        resultItem.currentStation = departure.value("stop").toString();
        if (departure.contains("rtTrack"))
            resultItem.platform = departure.value("rtTrack").toString();
        QString timeStr;
        if (departure.contains("rtTime")) {
            timeStr = departure.value("rtTime").toString();
            info << tr("Realtime info");
        } else {
            timeStr = departure.value("time").toString();
        }
        resultItem.time = QTime::fromString(timeStr, "HH:mm:ss");
        if (timetableSearchMode == Arrival)
            resultItem.destinationStation = departure.value("origin").toString();
        else
            resultItem.destinationStation = departure.value("direction").toString();
        resultItem.trainType = transportMode(product.value("catOutS").toString(),
                                                      product.value("catOutL").toString());
        QString lineNumber = product.value("num").toString();
        if (!lineNumber.isEmpty())
            resultItem.trainType += " " + lineNumber;

        // Operator info
        QString operatorName = product.value("operator").toString();
        QString operatorURL = product.value("operatorUrl").toString();
        if (!operatorName.isEmpty()) {
            if (operatorURL.isEmpty())
                info << operatorName;
            else
                info << "<a href=\"" + operatorURL + "\">" + operatorName + "</a>";
        }

        if (!info.isEmpty())
            resultItem.miscInfo = info.join("<br/>");

        timetable << resultItem;
    }

    emit timetableResult(timetable);
}

void ParserResRobot::parseStationsByName(QNetworkReply *networkReply)
{
    QByteArray allData = networkReply->readAll();
    qDebug() << "Reply:\n" << allData;

    QVariantMap doc = parseJson(allData);
    if (doc.isEmpty()) {
        emit errorOccured(tr("Cannot parse reply from the server"));
        return;
    }
    QVariantList stations = doc.value("StopLocation").toList();
    StationsList result;
    foreach (QVariant stationData, stations) {
        const QVariantMap& station = stationData.toMap();
        Station s;
        s.id = station.value("id").toString();
        s.name = station.value("name").toString();
        s.latitude = station.value("lat").toDouble();
        s.longitude = station.value("lon").toDouble();
        result.append(s);
    }

    emit stationsResult(result);
}

void ParserResRobot::parseStationsByCoordinates(QNetworkReply *networkReply)
{
    QByteArray allData = networkReply->readAll();
    qDebug() << "Reply:\n" << allData;

    QVariantMap doc = parseJson(allData);
    if (doc.isEmpty()) {
        emit errorOccured(tr("Cannot parse reply from the server"));
        return;
    }

    QVariantList stations = doc.value("StopLocation").toList();
    StationsList result;
    foreach (QVariant stationData, stations) {
        const QVariantMap& station = stationData.toMap();
        Station s;
        s.id = station.value("id").toString();
        s.name = station.value("name").toString();
        s.latitude = station.value("lat").toDouble();
        s.longitude = station.value("lon").toDouble();
        result.append(s);
    }

    emit stationsResult(result);
}

void ParserResRobot::parseSearchJourney(QNetworkReply *networkReply)
{
    QByteArray allData = networkReply->readAll();
    qDebug() << "Reply:\n" << allData;

    QVariantMap doc = parseJson(allData);
    if (doc.isEmpty()) {
        emit errorOccured(tr("Cannot parse reply from the server"));
        return;
    }

    searchEarlierReference = doc.value("scrB").toString();
    searchLaterReference = doc.value("scrF").toString();
    QVariantList journeyListData = doc.value("Trip").toList();

    cachedResults.clear();

    JourneyResultList *journeyList = new JourneyResultList();

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

// Parse info about one journey option. Store detailed info about segments for later use.
QList<JourneyDetailResultItem*> ParserResRobot::parseJourneySegments(const QVariantMap &journeyData)
{
    QList<JourneyDetailResultItem*> results;

    QVariantList segments = journeyData.value("LegList").toMap().value("Leg").toList();
    foreach (QVariant segmentData, segments)
    {
        const QVariantMap& segment = segmentData.toMap();
        JourneyDetailResultItem* resultItem = new JourneyDetailResultItem;

        // Departure
        QVariantMap departure = segment.value("Origin").toMap();
        resultItem->setDepartureStation(departure.value("name").toString());
        QDateTime departureDateTime;
        departureDateTime.setDate(QDate::fromString(departure.value("date").toString(), "yyyy-MM-dd"));
        departureDateTime.setTime(QTime::fromString(departure.value("time").toString(), "hh:mm:ss"));
        resultItem->setDepartureDateTime(departureDateTime);

        // Arrival
        QVariantMap arrival = segment.value("Destination").toMap();
        resultItem->setArrivalStation(arrival.value("name").toString());
        QDateTime arrivalDateTime;
        arrivalDateTime.setDate(QDate::fromString(arrival.value("date").toString(), "yyyy-MM-dd"));
        arrivalDateTime.setTime(QTime::fromString(arrival.value("time").toString(), "hh:mm:ss"));
        resultItem->setArrivalDateTime(arrivalDateTime);

        QStringList info;

        // Notes
        if (segment.contains("Notes")) {
            QVariantList notes = segment.value("Notes").toMap().value("Note").toList();
            foreach (QVariant note, notes) {
                QString hafasDescription(hafasAttribute(note.toMap().value("key").toString()));
                if (hafasDescription.isEmpty())
                    info.append(note.toMap().value("value").toString());
                else
                    info.append(hafasDescription);
            }
        }

        // Means of transportation
        QString distance;
        QString operatorInfo;
        QString transportMainType = segment.value("type").toString();
        if (transportMainType == "WALK" || transportMainType == "TRSF") {
            distance = segment.value("dist").toString();
            resultItem->setInternalData1("WALK");
            resultItem->setTrain(tr("Walk"));
        } else if (transportMainType == "JNY") {
            QVariantMap product = segment.value("Product").toMap();
            QString transportType = transportMode(product.value("catOutS").toString(),
                                                           product.value("catOutL").toString());
            QString lineNumber = product.value("num").toString();
            if (!lineNumber.isEmpty())
                transportType += " " + lineNumber;
            QString operatorName = product.value("operator").toString();
            QString operatorURL = product.value("operatorUrl").toString();
            if (!operatorName.isEmpty()) {
                if (operatorURL.isEmpty())
                    operatorInfo = operatorName;
                else
                    operatorInfo = "<a href=\"" + operatorURL + "\">" + operatorName + "</a>";
            }
            resultItem->setTrain(transportType);
        } else {
            while (!results.isEmpty())
                delete results.takeFirst();
            delete resultItem;
            break;
        }

        if (!distance.isEmpty())
            resultItem->setInfo(distance + " m");
        else if (!operatorInfo.isEmpty() && !info.isEmpty())
            resultItem->setInfo(operatorInfo + "<br/>" + info.join(", "));
        else if (!operatorInfo.isEmpty())
            resultItem->setInfo(operatorInfo);
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
    parseSearchJourney(networkReply);
}

void ParserResRobot::parseSearchEarlierJourney(QNetworkReply *networkReply)
{
    parseSearchJourney(networkReply);
}

void ParserResRobot::parseJourneyDetails(QNetworkReply *networkReply)
{
    Q_UNUSED(networkReply)
    // Should never happen since we get all info about all journeys at once
    // and therefore never need to send any journeyDetailsRequest.
}

QString ParserResRobot::hafasAttribute(const QString& code)
{
    if (hafasAttributes.contains(code))
        return hafasAttributes[code];
    else
        return "";
}

QString ParserResRobot::transportMode(const QString& code, const QString& fallback)
{
    if (specificTransportModes.contains(code)) {
        return specificTransportModes[code];
    } else if (code.size() >= 1 && generalTransportModes.contains(code.left(1))) {
        return generalTransportModes[code.left(1)];
    } else {
        qDebug() << "Didn't find transport mode" << code << ", falling back to " << fallback;
        return fallback;
    }
}

QString ParserResRobot::formatRestrictions(int restriction)
{
    switch (restriction) {
    default:
    case ALL_TRANSPORT_MODES:
        break;
    case TRAIN_PUB_TRANS_NOT_EXP_BUS:
        return QString::number(HIGH_SPEED_TRAIN + REGIONAL_TRAIN + LOCAL_TRAIN +
                               SUBWAY + TRAM + BUS + FERRY);
    case EXP_BUS_PUB_TRANS_NOT_TRAIN:
        return QString::number(EXPRESS_BUS + SUBWAY + TRAM + BUS + FERRY);
    }
    return "";
}

QString ParserResRobot::stationIDv2(QString id)
{
    // A version 1 ID looks something like "007459175", which for v2 should look like "740059175"
    while (!id.isEmpty() && id.at(0) == '0')
        id.remove(0, 1);
    if (id.size() >= 2 && id.size() != 9) {
        id.insert(2, "00");
    }
    return id;
}
