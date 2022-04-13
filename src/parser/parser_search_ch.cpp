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
#include <QJsonDocument>
#else
#include <QScriptEngine>
#endif
#include <QLocale>
#include <QRegExp>
#include <QVariantMap>
#include <QFile>

#include "parser_search_ch.h"

using namespace parser_search_ch;

/*
 * TimetableRow Class
 */

TimetableRow::TimetableRow(const QVariantMap& stop)
    : stop(stop)
{
    const QVariant& lat = stop.value("lat");
    const QVariant& lon = stop.value("lon");

    if (!lon.isNull() && !lat.isNull()) {
        timetable.latitude = lat.toDouble();
        timetable.longitude = lon.toDouble();
    }
}

void TimetableRow::load(const QVariantMap& departure)
{
    const QVariantMap& dest = departure.value("terminal").toMap();

    timetable.destinationStation = dest.value("name").toString();
    timetable.time = ParserSearchCH::tsFromMap(departure, "time").time();

    this->loadTrainType(departure);
    this->loadDelay(departure);
    this->loadDeparturePlatform(departure);
}

void TimetableRow::appendTo(TimetableEntriesList& tt) {
    tt.append(timetable);
}

void TimetableRow::loadTrainTypeWithoutLine(const QVariant& trainType)
{
    const QString& type = trainType.toString();

    if (type == "ship") {
        timetable.trainType = tr("Ship");
    } else if (type == "funicular") {
        timetable.trainType = tr("Funicular");
    } else if (type == "cablecar") {
        timetable.trainType = tr("Cablecar");
    } else if (type == "gondola") {
        timetable.trainType = tr("Gondola");
    }
}

void TimetableRow::loadTrainType(const QVariantMap& departure)
{
    const QVariant& line = departure.value("line");
    const QVariant& type = departure.value("type");

    if (type.toString() == "bus") {
        timetable.trainType = tr("Bus %1").arg(line.toString());
    } else if (type.toString() == "post") {
        timetable.trainType = tr("Bus %1").arg(line.toString());
    } else if (type.toString() == "tram") {
        timetable.trainType = tr("Tram %1").arg(line.toString());
    } else if (line.isNull()) {
        this->loadTrainTypeWithoutLine(type);
    } else {
        timetable.trainType = line.toString();
    }
}

void TimetableRow::loadDeparturePlatform(const QVariantMap& departure)
{
    const QVariant& platform = departure.value("track");

    if (platform.isNull()) {
        timetable.currentStation = stop.value("name").toString();
    } else {
        timetable.platform = platform.toString();
    }
}

void TimetableRow::loadDelay(const QVariantMap& departure)
{
    const QVariant& delay = departure.value("dep_delay");

    if (delay.isNull()) {
        return;
    }

    const QString& minutes = delay.toString();

    if (minutes == "X") {
        timetable.miscInfo = tr("Cancelled");
    } else if (minutes != "+0") {
        timetable.miscInfo = tr("Departure delayed: %1'").arg(minutes);
    }
}

/*
 * Class TrainTypeList
 */

TrainTypeList::TrainTypeList() :
    QObject()
{
}

void TrainTypeList::appendUnique(const QString& type)
{
    if (!types.contains(type)) {
        types.append(type);
    }
}

void TrainTypeList::append(const QString& type)
{
    if (type == "B") {
        appendUnique(tr("Bus"));
    } else if (type == "BN") {
        appendUnique(tr("Bus"));
    } else if (type == "T") {
        appendUnique(tr("Tram"));
    } else if (type == "FUN") {
        appendUnique(tr("Funi"));
    } else if (type == "GB") {
        appendUnique(tr("Gondola"));
    } else if (type == "PB") {
        appendUnique(tr("Cableway"));
    } else if (type == "BAT") {
        appendUnique(tr("Ship"));
    } else {
        appendUnique(type);
    }
}

QString TrainTypeList::toString()
{
    if (types.length() > 0) {
        return types.join(" ");
    } else {
        return tr("Walk");
    }
}

/*
 * Class JourneySegment
 */

JourneySegment::JourneySegment(const QVariantMap& leg, const QVariant& exit)
{
    this->setDepartureStation(leg.value("name").toString());
    this->setDirection(leg.value("terminal").toString());

    this->loadDepartureTrack(leg);
    this->loadDelay(leg);
    this->loadTrainType(leg);
    this->loadExit(leg, exit.toMap());
 
//  this->setInternalData1("UNUSED");
//  this->setInternalData2("UNUSED");
}

void JourneySegment::loadDepartureTrack(const QVariantMap& leg)
{
    const QVariant& track = leg.value("track");

    if (!track.isNull()) {
        this->setDepartureInfo(tr("Track %1").arg(track.toString()));
    }
}

void JourneySegment::loadDelay(const QVariantMap& leg)
{
    const QVariant& delay = leg.value("dep_delay");

    if (!delay.isNull()) {
        if (delay.toString() == "X") {
            this->setInfo(tr("Train cancelled"));
        } else if (delay.toString() != "+0") {
            QString dt = tr("Departure delayed: %1'").arg(delay.toString());
            this->setInfo(dt);
        }
    }
}

void JourneySegment::loadTrainType(const QVariantMap& leg)
{
    const QVariant& line(leg.value("line"));
    const QVariant& type(leg.value("type"));

    if (line.toString() == "Bus") {
        this->setTrain(tr("Bus"));
    } else if (type.toString() == "bus") {
        this->setTrain(tr("Bus %1").arg(line.toString()));
    } else if (type.toString() == "post") {
        this->setTrain(tr("Bus %1").arg(line.toString()));
    } else if (type.toString() == "night_bus") {
        this->setTrain(tr("Bus %1").arg(line.toString()));
    } else if (type.toString() == "tram") {
        this->setTrain(tr("Tram %1").arg(line.toString()));
    } else if (!line.isNull()) {
        this->setTrain(line.toString());
    } else {
        this->setTrain(leg.value("type_name").toString());
    }
}

void JourneySegment::loadExit(const QVariantMap& leg, const QVariantMap& exit)
{
    this->setArrivalStation(exit.value("name").toString());

    const QVariant& track = exit.value("track");
    if (!track.isNull()) {
        this->setArrivalInfo(tr("Track %1").arg(track.toString()));
    }

    this->setDepartureDateTime(ParserSearchCH::tsFromMap(leg, "departure"));
    this->setArrivalDateTime(ParserSearchCH::tsFromMap(exit, "arrival"));
}

/*
 * Class JourneyConnection
 */

JourneyConnection::JourneyConnection(int id)
{
    this->setId(QString::number(id++));
//  this->setInternalData1("UNUSED");
//  this->setInternalData2("UNUSED");
}

void JourneyConnection::load(const QVariantMap &dataRow)
{
    this->departure = ParserSearchCH::tsFromMap(dataRow, "departure");
    this->arrival = ParserSearchCH::tsFromMap(dataRow, "arrival");
    legs = dataRow.value("legs").toList();
    const QVariant& delay = dataRow.value("dep_delay");

    this->setDate(this->departure.date());
    this->loadDepartureTime(departure, dataRow.value("dep_delay"));
    this->loadArrivalTime(arrival, dataRow.value("arr_delay"));
    this->loadDuration(dataRow.value("duration"));
    this->loadTrainTypes(legs);
    this->countTransfers(legs);
    this->checkIfCancelled(delay);
}

void JourneyConnection::loadDepartureTime(const QDateTime& ts,
        const QVariant& delay)
{
    JourneyConnectionTime dep(ts, delay);
    this->setDepartureTime(dep.toString());
}

void JourneyConnection::loadArrivalTime(const QDateTime& ts,
        const QVariant& delay)
{
    JourneyConnectionTime arr(ts, delay);
    this->setArrivalTime(arr.toString());
}

void JourneyConnection::loadDuration(const QVariant& duration)
{
    int min = duration.toInt() / 60;
    this->setDuration(QString("%1:%2").arg(min/60).arg(min%60,2,10,QChar('0')));
}

void JourneyConnection::loadTrainTypes(const QVariantList& legs)
{
    TrainTypeList types;

    Q_FOREACH (const QVariant& leg, legs) {
        const QVariantMap& details = leg.toMap();
        const QVariant& trainType = details.value("*G");

        if (trainType.isNull()) {
            const QVariant& time = details.value("runningtime");
            const QVariant& type = details.value("type");

            if ((type.toString() == "walk") && time.toInt() > 600) {
                types.append(tr("Walk"));
            }
        } else {
            types.append(trainType.toString());
        }
    }

    this->setTrainType(types.toString());
}

void JourneyConnection::countTransfers(const QVariantList& legs)
{
    int transfers = 0;

    Q_FOREACH (const QVariant& leg, legs) {
        const QVariantMap& details = leg.toMap();
        const QVariant& type = details.value("type");

        if (!type.isNull()) {
            if (type.toString() == "walk") {
                continue;
            }
            transfers++;
        }
    }

    if (transfers > 0) {
        this->setTransfers(QString::number(transfers - 1));
    }
}

void JourneyConnection::checkIfCancelled(const QVariant& dep_delay)
{
    if (!dep_delay.isNull()) {
        if (dep_delay.toString() == "X") {
            this->setMiscInfo("Cancelled");
        }
    }
}

/*
 * Class JourneyConnectionDetails
 */

JourneyConnectionDetails::JourneyConnectionDetails(JourneyConnection* con,
        const JourneySearch& lastSearch)
{
    this->loadSegments(con);

    this->setId(con->id());
    this->setDepartureStation(lastSearch.from.name);
    this->setArrivalStation(lastSearch.to.name);

    if (lastSearch.via.valid) {
        this->setViaStation(lastSearch.via.name);
    }

    this->setDuration(con->duration());
    this->setDepartureDateTime(con->departure);
    this->setArrivalDateTime(con->arrival);
//  this->setInfo("UNUSED");
}

void JourneyConnectionDetails::loadSegments(JourneyConnection* con)
{
    Q_FOREACH (const QVariant& row, con->legs) {
        const QVariantMap& leg(row.toMap());
        const QVariant& exit(leg.value("exit"));

        /* The last segment (arrival) has no exit, skip it */
        if (exit.isNull()) {
            continue;
        }

        this->appendItem(new JourneySegment(leg, exit));
    }
}

/*
 * Class JourneyConnectionTime
 */

JourneyConnectionTime::JourneyConnectionTime(const QDateTime& dateTime,
        const QVariant& delayValue)
    : ts(dateTime), delay(delayValue) {}

QString JourneyConnectionTime::toString()
{
    QString time = ts.toLocalTime().time().toString("HH:mm");

    if (delay.isNull() || (delay.toString() == "+0")) {
        return time;
    } else {
        return QString("%1 %2'").arg(time, delay.toString());
    }
}

/*
 * Class JourneySearchResult
 */

JourneySearchResult::JourneySearchResult(const JourneySearch& search)
{
    this->loadTimeInfo(search.when, search.mode);

    this->setDepartureStation(search.from.name);
    this->setArrivalStation(search.to.name);

    if (search.via.valid) {
        this->setViaStation(search.via.name);
    }
}

void JourneySearchResult::loadTimeInfo(const QDateTime& when,
        const ParserAbstract::Mode& mode)
{
    QString info;

    if (mode == ParserAbstract::Arrival) {
        info = tr("Arrivals %1");
    } else {
        info = tr("Departures %1");
    }

    QString time(when.toString(tr("ddd MMM d, HH:mm")));
    this->setTimeInfo(info.arg(time));
}

/*
 * Class ParserSearchCH
 */

ParserSearchCH::ParserSearchCH(QObject *parent) :
    ParserAbstract(parent)
{
}

bool ParserSearchCH::supportsGps()
{
    return true;
}

bool ParserSearchCH::supportsVia()
{
    return true;
}

bool ParserSearchCH::supportsTimeTable()
{
    return true;
}

QStringList ParserSearchCH::getTrainRestrictions()
{
    QStringList result;
    result.append(tr("All"));
    result.append(tr("Train"));
    result.append(tr("Tram"));
    result.append(tr("Ship"));
    result.append(tr("Bus"));
    result.append(tr("Cableway"));

    return result;
}


/*
 * Common code
 */

#if defined(BUILD_FOR_QT5)
void ParserSearchCH::addRestrictionsToQuery(QUrlQuery& query, int restrictions)
#else
void ParserSearchCH::addRestrictionsToQuery(QUrl& query, int restrictions)
#endif
{
    switch (restrictions) {
        case 1:
            query.addQueryItem("transportation_types", "train");
            break;
        case 2:
            query.addQueryItem("transportation_types", "tram");
            break;
        case 3:
            query.addQueryItem("transportation_types", "ship");
            break;
        case 4:
            query.addQueryItem("transportation_types", "bus");
            break;
        case 5:
            query.addQueryItem("transportation_types", "cableway");
            break;
    }
}

/* Inspired by ParserAbstract::parseJson */
QVariantList ParserSearchCH::parseJsonList(const QByteArray &json) const
{
    QVariantList doc;
#ifdef BUILD_FOR_QT5
    doc = QJsonDocument::fromJson(json).toVariant().toList();
#else
    QString utf8(QString::fromUtf8(json));

    // Validation of JSON according to RFC 4627, section 6
    QString tmp(utf8);
    if (tmp.replace(QRegExp("\"(\\\\.|[^\"\\\\])*\""), "")
           .contains(QRegExp("[^,:{}\\[\\]0-9.\\-+Eaeflnr-u \\n\\r\\t]")))
        return doc;

    QScriptEngine *engine = new QScriptEngine();
    doc = engine->evaluate("(" + utf8 + ")").toVariant().toList();
    delete engine;
#endif

    return doc;
}

void ParserSearchCH::sendRequest(QUrl url)
{
    QList<QPair<QByteArray,QByteArray> > additionalHeaders;
    additionalHeaders.append(QPair<QByteArray,QByteArray>("Content-Type", "application/json"));

    qDebug() << "Sending request to " << url.toString();
    sendHttpRequest(url, NULL, additionalHeaders);
}

QDateTime ParserSearchCH::tsFromMap(const QVariantMap& map, const QString& key)
{
    QString departure = map.value(key).toString();
    QDateTime dt = QDateTime::fromString(departure, "yyyy-MM-dd HH:mm:ss");
#ifdef BUILD_FOR_QT5
    dt.setTimeZone(QTimeZone("Europe/Zurich"));
#endif
    return dt.toLocalTime();
}


/*
 * Station lookup (by name or coordinates)
 */

void ParserSearchCH::findStationsByName(const QString &stationName)
{
    if (stationName.length() < 2) {
        return;
    }

    if (currentRequestState != FahrplanNS::noneRequest) {
        return;
    }

    currentRequestState = FahrplanNS::stationsByNameRequest;

    QUrl url("http://timetable.search.ch/api/completion.json");
#if defined(BUILD_FOR_QT5)
    QUrlQuery query;
#else
    QUrl query;
#endif
    query.addQueryItem("term", stationName);
#if defined(BUILD_FOR_QT5)
    url.setQuery(query);
#else
    url.setQueryItems(query.queryItems());
#endif
    sendRequest(url);
}

void ParserSearchCH::findStationsByCoordinates(qreal longitude, qreal latitude)
{
    if (currentRequestState != FahrplanNS::noneRequest) {
        return;
    }

    currentRequestState = FahrplanNS::stationsByCoordinatesRequest;

    QUrl url("http://timetable.search.ch/api/completion.json");
#if defined(BUILD_FOR_QT5)
    QUrlQuery query;
#else
    QUrl query;
#endif
    QString pos = QString::number(latitude) + "," + QString::number(longitude);
    query.addQueryItem("latlon", pos);

#if defined(BUILD_FOR_QT5)
    url.setQuery(query);
#else
    url.setQueryItems(query.queryItems());
#endif
    sendRequest(url);
}

void ParserSearchCH::parseStationRow(StationsList& rows, const QVariantMap& row)
{
    const QVariant& icon = row.value("iconclass");
    const QVariant& label = row.value("label");

    /* Ignore street addresses */
    if ((icon.toString() == "sl-icon-type-adr") ||
        (icon.toString() == "sl-icon-tel-business")) {
        return;
    }

    Station s;
    if (icon.toString() == "sl-icon-type-train") {
        s.miscInfo = tr("Train station");
    } else if (icon.toString() == "sl-icon-type-strain") {
        s.miscInfo = tr("Train station");
    } else if (icon.toString() == "sl-icon-type-tram") {
        s.miscInfo = tr("Tram stop");
    } else if (icon.toString() == "sl-icon-type-ship") {
        s.miscInfo = tr("Port");
    } else if (icon.toString() == "sl-icon-type-bus") {
        s.miscInfo = tr("Bus stop");
    } else if (icon.toString() == "sl-icon-type-funicular") {
        s.miscInfo = tr("Funicular");
    } else if (icon.toString() == "sl-icon-type-gondola") {
        s.miscInfo = tr("Gondola");
    } else if (icon.toString() == "sl-icon-type-cablecar") {
        s.miscInfo = tr("Cablecar");
    } else {
        qDebug() << "Unknown icon type: " << icon.toString();
    }

    if (!label.isNull())
    {
        const QString& name = label.toString();
        s.id = name;
        s.name = name;
        rows.append(s);
    }
}

void ParserSearchCH::parseStations(QNetworkReply *networkReply)
{
    QByteArray allData(networkReply->readAll());

    QVariantList stations = parseJsonList(allData);
    if (stations.isEmpty()) {
        qDebug() << "Invalid reply:" << allData;
        emit errorOccured(tr("Cannot parse reply from the server"));
        return;
    }

    StationsList results;
    Q_FOREACH (const QVariant& featureData, stations) {
        parseStationRow(results, featureData.toMap());
    }

    emit stationsResult(results);
}

void ParserSearchCH::parseStationsByCoordinates(QNetworkReply *networkReply)
{
    parseStations(networkReply);
}

void ParserSearchCH::parseStationsByName(QNetworkReply *networkReply)
{
    parseStations(networkReply);
}


/*
 * Timetable (station board)
 */

void ParserSearchCH::getTimeTableForStation(const Station &station,
        const Station &direction, const QDateTime &when, Mode mode,
        int restrictions)
{
    Q_UNUSED(direction);

    if (currentRequestState != FahrplanNS::noneRequest) {
        return;
    }

    currentRequestState = FahrplanNS::getTimeTableForStationRequest;

    QUrl url("http://timetable.search.ch/api/stationboard.json");
#if defined(BUILD_FOR_QT5)
    QUrlQuery query;
#else
    QUrl query;
#endif
    query.addQueryItem("stop", station.name);
    query.addQueryItem("limit", "15");
    query.addQueryItem("date", when.toString("MM/dd/yyyy"));
    query.addQueryItem("time", when.toString("hh:mm"));
    query.addQueryItem("show_tracks", "1");
    query.addQueryItem("show_delays", "1");

    addRestrictionsToQuery(query, restrictions);

    if (mode == Arrival) {
        query.addQueryItem("mode", "arrival");
    }
#if defined(BUILD_FOR_QT5)
    url.setQuery(query);
#else
    url.setQueryItems(query.queryItems());
#endif

    sendRequest(url);
}

void ParserSearchCH::parseTimeTable(QNetworkReply *networkReply)
{
    TimetableEntriesList timetable;
    QByteArray allData(networkReply->readAll());

    QVariantMap doc = parseJson(allData);
    if (doc.isEmpty()) {
        qDebug() << "Invalid reply:" << allData;
        emit errorOccured(tr("Cannot parse reply from the server"));
        return;
    }

    const QVariantMap& stop = doc.value("stop").toMap();
    const QVariantList departures = doc.value("connections").toList();

    Q_FOREACH (const QVariant& row, departures) {
        TimetableRow entry(stop);
        entry.load(row.toMap());
        entry.appendTo(timetable);
    }

    emit timetableResult(timetable);
}

/*
 * Journey (route)
 */

void ParserSearchCH::searchJourney(const Station &from, const Station &via,
    const Station &to, const QDateTime &when, Mode mode, int restrictions)
{
    if (currentRequestState != FahrplanNS::noneRequest) {
        return;
    }

    currentRequestState = FahrplanNS::searchJourneyRequest;

    lastJourneySearch.mode = mode;
    lastJourneySearch.from = from;
    lastJourneySearch.to = to;
    lastJourneySearch.via = via;
    lastJourneySearch.when = when;
    lastJourneySearch.restrictions = restrictions;

    QUrl url("http://timetable.search.ch/api/route.json");
#if defined(BUILD_FOR_QT5)
    QUrlQuery query;
#else
    QUrl query;
#endif
    query.addQueryItem("from", from.name);
    query.addQueryItem("to", to.name);
    query.addQueryItem("date", when.toString("MM/dd/yyyy"));
    query.addQueryItem("time", when.toString("hh:mm"));
    query.addQueryItem("show_delays", "1");

    if (via.valid) {
        query.addQueryItem("via", via.name);
    }

    addRestrictionsToQuery(query, restrictions);

    if (mode == Arrival) {
        query.addQueryItem("time_type", "arrival");
    }
#if defined(BUILD_FOR_QT5)
    url.setQuery(query);
#else
    url.setQueryItems(query.queryItems());
#endif

    sendRequest(url);
}

void ParserSearchCH::parseSearchJourney(QNetworkReply *networkReply)
{
    JourneySearchResult *result = new JourneySearchResult(lastJourneySearch);
    QByteArray jsonData(networkReply->readAll());

    QVariantMap doc = parseJson(jsonData);
    if (doc.isEmpty()) {
        qDebug() << "Invalid reply:" << jsonData;
        emit errorOccured(tr("Cannot parse reply from the server"));
        return;
    }

    QVariantList rows = doc.value("connections").toList();

    while (!details.isEmpty()) {
        delete details.takeFirst();
    }

    Q_FOREACH (const QVariant& row, rows) {
        JourneyConnection* conn = new JourneyConnection(result->itemcount());
        conn->load(row.toMap());

        details.append(new JourneyConnectionDetails(conn, lastJourneySearch));
        result->appendItem(conn);
    }

    emit journeyResult(result);
}

void ParserSearchCH::getJourneyDetails(const QString &id)
{
    int i = id.toInt();

    if (details.length() > i) {
        emit journeyDetailsResult(details[i]);
    } else {
        emit errorOccured(tr("No journey details found."));
    }
}

void ParserSearchCH::searchJourneyLater()
{
    QDateTime nextQueryTime;

    if (details.length() > 0) {
        // last search found something, skip 1 minute ahead from last result
        JourneyDetailResultList* result = details.last();
        nextQueryTime = result->departureDateTime().addSecs(60);
    } else {
        // last search found nothing, skip 1 hour ahead from search time
        nextQueryTime = lastJourneySearch.when.addSecs(3600);
    }

    searchJourney(lastJourneySearch.from, lastJourneySearch.via,
            lastJourneySearch.to, nextQueryTime, Departure,
            lastJourneySearch.restrictions);
}

void ParserSearchCH::searchJourneyEarlier()
{
    QDateTime nextQueryTime;

    if (details.length() > 0) {
        // last search found something, skip 1 minute ahead from last result
        JourneyDetailResultList* result = details.first();
        nextQueryTime = result->arrivalDateTime().addSecs(-60);
    } else {
        // last search found nothing, skip 1 hour ahead from search time
        nextQueryTime = lastJourneySearch.when.addSecs(-3600);
    }

    searchJourney(lastJourneySearch.from, lastJourneySearch.via,
            lastJourneySearch.to, nextQueryTime, Arrival,
            lastJourneySearch.restrictions);
}
