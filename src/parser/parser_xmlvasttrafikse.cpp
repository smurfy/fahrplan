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

#include "parser_xmlvasttrafikse.h"

#include <QDebug>

#include <QtCore/QUrl>
#include <QtNetwork/QNetworkReply>
#include <QtXml/QDomDocument>

#if defined(BUILD_FOR_QT5)
    #include <QUrlQuery>
#endif

const qlonglong ParserXmlVasttrafikSe::TRIP_RTDATA_NONE = -1;
const qlonglong ParserXmlVasttrafikSe::TRIP_RTDATA_ONTIME = 1;
const qlonglong ParserXmlVasttrafikSe::TRIP_RTDATA_WARNING = 2;


QHash<QString, JourneyDetailResultList *> cachedJourneyDetails;


#define getAttribute(node, key) (node.attributes().namedItem(key).toAttr().value())

ParserXmlVasttrafikSe::ParserXmlVasttrafikSe(QObject *parent)
    : ParserAbstract(parent), lastJourneyResultList(NULL), apiKey(QLatin1String("47c5abaf-49d6-4c23-a1bd-b2e2766c4de7")), baseRestUrl(QLatin1String("http://api.vasttrafik.se/bin/rest.exe/v1/"))
{
    m_searchJourneyParameters.isValid = false;
    m_timeTableForStationParameters.isValid = false;
}

ParserXmlVasttrafikSe::~ParserXmlVasttrafikSe()
{
    clearJourney();
}

void ParserXmlVasttrafikSe::clearJourney()
{
    if (lastJourneyResultList) {
        delete lastJourneyResultList;
        lastJourneyResultList = NULL;
    }

    for (QHash<QString, JourneyDetailResultList *>::Iterator it = cachedJourneyDetails.begin(); it != cachedJourneyDetails.end();) {
        JourneyDetailResultList *jdrl = it.value();
        it = cachedJourneyDetails.erase(it);
        delete jdrl;
    }
}

void ParserXmlVasttrafikSe::getTimeTableForStation(const Station &currentStation, const Station &, const QDateTime &dateTime, Mode mode, int)
{
    if (currentRequestState != FahrplanNS::noneRequest)
        return;
    currentRequestState = FahrplanNS::getTimeTableForStationRequest;

    m_timeTableForStationParameters.isValid = false;

    QUrl uri(baseRestUrl + (mode == Departure ? QLatin1String("departureBoard") : QLatin1String("arrivalBoard")));

#if defined(BUILD_FOR_QT5)
    QUrlQuery query;
#else
    QUrl query;
#endif
    query.addQueryItem("authKey", apiKey);
    query.addQueryItem("format", "xml");
    query.addQueryItem("date", dateTime.toString("yyyy-MM-dd"));
    query.addQueryItem("time", dateTime.toString("hh:mm"));
    query.addQueryItem("id", currentStation.id.toString());
    query.addQueryItem("useVas", "1");
    query.addQueryItem("useLDTrain", "0");
    query.addQueryItem("useRegTrain", "1");
    query.addQueryItem("useBus", "1");
    query.addQueryItem("useBoat", "1");
    query.addQueryItem("useTram", "1");
    query.addQueryItem("excludeDR", "1");
#if defined(BUILD_FOR_QT5)
    uri.setQuery(query);
#else
    uri.setQueryItems(query.queryItems());
#endif
    sendHttpRequest(uri);
}

void ParserXmlVasttrafikSe::findStationsByName(const QString &stationName)
{
    qDebug() << "ParserXmlVasttrafikSe::findStationsByName(stationName" << stationName << ")";

    if (currentRequestState != FahrplanNS::noneRequest)
        return;
    currentRequestState = FahrplanNS::stationsByNameRequest;

    QUrl uri(baseRestUrl + QLatin1String("location.name"));
#if defined(BUILD_FOR_QT5)
    QUrlQuery query;
#else
    QUrl query;
#endif
    query.addQueryItem("authKey", apiKey);
    query.addQueryItem("format", "xml");
    query.addQueryItem("input", stationName);
#if defined(BUILD_FOR_QT5)
    uri.setQuery(query);
#else
    uri.setQueryItems(query.queryItems());
#endif
    sendHttpRequest(uri);
}

void ParserXmlVasttrafikSe::findStationsByCoordinates(qreal longitude, qreal latitude)
{
    qDebug() << "ParserXmlVasttrafikSe::findStationsByCoordinates(longitude=" << longitude << ", latitude=" << latitude << ")";

    if (currentRequestState != FahrplanNS::noneRequest)
        return;
    currentRequestState = FahrplanNS::stationsByCoordinatesRequest;

    QUrl uri(baseRestUrl + QLatin1String("location.nearbystops"));
#if defined(BUILD_FOR_QT5)
    QUrlQuery query;
#else
    QUrl query;
#endif
    query.addQueryItem("authKey", apiKey);
    query.addQueryItem("format", "xml");
    query.addQueryItem("originCoordLat", QString::number(latitude));
    query.addQueryItem("originCoordLong", QString::number(longitude));
    query.addQueryItem("maxNo", QString::number(5));
#if defined(BUILD_FOR_QT5)
    uri.setQuery(query);
#else
    uri.setQueryItems(query.queryItems());
#endif
    sendHttpRequest(uri);
}

void ParserXmlVasttrafikSe::searchJourney(const Station &departureStation, const Station &viaStation, const Station &arrivalStation, const QDateTime &dateTime, ParserAbstract::Mode mode, int trainrestrictions)
{
    qDebug() << "ParserXmlVasttrafikSe::searchJourney(departureStation=" << departureStation.name << ", arrivalStation=" << arrivalStation.name << ", viaStation=" << viaStation.name << ", dateTime=" << dateTime.toString() << ", mode=" << mode << ", trainrestrictions=" << trainrestrictions << ")";

    if (currentRequestState != FahrplanNS::noneRequest)
        return;
    currentRequestState = FahrplanNS::searchJourneyRequest;

    m_searchJourneyParameters.isValid = false;
    m_searchJourneyParameters.departureStation = departureStation;
    m_searchJourneyParameters.arrivalStation = arrivalStation;
    m_searchJourneyParameters.viaStation = viaStation;
    m_searchJourneyParameters.dateTime = dateTime;
    m_searchJourneyParameters.mode = mode;
    m_searchJourneyParameters.trainrestrictions = trainrestrictions;

    QUrl uri(baseRestUrl + QLatin1String("trip"));
#if defined(BUILD_FOR_QT5)
    QUrlQuery query;
#else
    QUrl query;
#endif
    query.addQueryItem("authKey", apiKey);
    query.addQueryItem("format", "xml");
    query.addQueryItem("date", dateTime.toString("yyyy-MM-dd"));
    query.addQueryItem("time", dateTime.toString("hh:mm"));
    query.addQueryItem("originId", departureStation.id.toString());
    if (viaStation.valid)
        query.addQueryItem("viaId", viaStation.id.toString());
    if (mode == Arrival)
        query.addQueryItem("searchForArrival", "yes");
    query.addQueryItem("destId", arrivalStation.id.toString());
    query.addQueryItem("useVas", "1");
    query.addQueryItem("useLDTrain", "0");
    query.addQueryItem("useRegTrain", "1");
    query.addQueryItem("useBus", "1");
    query.addQueryItem("useBoat", "1");
    query.addQueryItem("useTram", "1");
    query.addQueryItem("excludeDR", "1");
    query.addQueryItem("numTrips", "5");
#if defined(BUILD_FOR_QT5)
    uri.setQuery(query);
#else
    uri.setQueryItems(query.queryItems());
#endif
    sendHttpRequest(uri);
}

void ParserXmlVasttrafikSe::getJourneyDetails(const QString &id)
{
    qDebug() << "ParserXmlVasttrafikSe::getJourneyDetails(id=" << id << ")";

    if (currentRequestState != FahrplanNS::noneRequest)
        return;

    emit journeyDetailsResult(cachedJourneyDetails.value(id, NULL));
}

bool ParserXmlVasttrafikSe::supportsGps()
{
    return true;
}

bool ParserXmlVasttrafikSe::supportsVia()
{
    return true;
}

bool ParserXmlVasttrafikSe::supportsTimeTable()
{
    return true;
}

bool ParserXmlVasttrafikSe::supportsTimeTableDirection()
{
    return false;
}

void ParserXmlVasttrafikSe::parseStationsByName(QNetworkReply *networkReply)
{
    qDebug() << "ParserXmlVasttrafikSe::parseStationsByName(networkReply.url()=" << networkReply->url().toString() << ")";

    StationsList result;
    QTextStream ts(networkReply->readAll());
    ts.setCodec("UTF-8");
    const QString xmlRawtext = ts.readAll();

    QDomDocument doc("result");
    if (doc.setContent(xmlRawtext, false)) {
        QDomNodeList locationList = doc.elementsByTagName("StopLocation");
        for (unsigned int i = 0; i < locationList.length(); ++i) {
            QDomNode locationNode = locationList.item(i);

            Station item;
            item.id = getAttribute(locationNode, "id");
            item.name = getAttribute(locationNode, "name");
            item.latitude = getAttribute(locationNode, "lat").toFloat();
            item.longitude = getAttribute(locationNode, "lon").toFloat();

            result << item;
        }
    }

    if (m_timeTableForStationParameters.isValid) {
        getTimeTableForStation(m_timeTableForStationParameters.currentStation, m_timeTableForStationParameters.directionStation, m_timeTableForStationParameters.dateTime, m_timeTableForStationParameters.mode, m_timeTableForStationParameters.trainrestrictions);
    } else if (m_searchJourneyParameters.isValid) {
        searchJourney(m_searchJourneyParameters.departureStation, m_searchJourneyParameters.arrivalStation, m_searchJourneyParameters.viaStation, m_searchJourneyParameters.dateTime, m_searchJourneyParameters.mode, m_searchJourneyParameters.trainrestrictions);
    } else {
        emit stationsResult(result);
    }
}

void ParserXmlVasttrafikSe::parseStationsByCoordinates(QNetworkReply *networkReply)
{
    qDebug() << "ParserXmlVasttrafikSe::parseStationsByCoordinates(networkReply.url()=" << networkReply->url().toString() << ")";
    parseStationsByName(networkReply);
}

void ParserXmlVasttrafikSe::parseTimeTable(QNetworkReply *networkReply)
{
    qDebug() << "ParserXmlVasttrafikSe::parseTimeTable(networkReply.url()=" << networkReply->url().toString() << ")";

    TimetableEntriesList result;
    QTextStream ts(networkReply->readAll());
    ts.setCodec("UTF-8");
    const QString xmlRawtext = ts.readAll();

    QDomDocument doc("result");
    if (doc.setContent(xmlRawtext, false)) {
        bool isArrival = false;
        QDomNodeList nodeList = doc.elementsByTagName("Departure");
        if (nodeList.isEmpty()) {
            nodeList = doc.elementsByTagName("Arrival");
            isArrival = true;
        }
        for (unsigned int i = 0; i < nodeList.length(); ++i) {
            QDomNode node = nodeList.item(i);
            TimetableEntry item;

            item.destinationStation = getAttribute(node, isArrival ? "origin" : "direction");
            item.platform = getAttribute(node, "track");
            const QString connectionName = i18nConnectionType(getAttribute(node, "name"));
            const QString fgColor = getAttribute(node, "fgColor");
            const QString bgColor = getAttribute(node, "bgColor");
            if (!fgColor.isEmpty() && !bgColor.isEmpty())
                item.trainType = QString::fromLatin1("<span style=\"color:%2; background-color: %3;\">%1</span>").arg(connectionName).arg(fgColor).arg(bgColor);
            else
                item.trainType = connectionName;
            const QTime scheduledTime = QTime::fromString(getAttribute(node, "time"), QLatin1String("hh:mm"));
            item.time = scheduledTime;
            const QString realTimeStr = getAttribute(node, "rtTime");
            if (!realTimeStr.isEmpty()) {
                const QTime realTimeTime = QTime::fromString(realTimeStr, QLatin1String("hh:mm"));
                const int minutesTo = scheduledTime.msecsTo(realTimeTime) / 60000;
                if (minutesTo > 3)
                    item.miscInfo = tr("<span style=\"color:#b30;\">%1 min late</span>").arg(minutesTo);
                else
                    item.miscInfo = tr("<span style=\"color:#093; font-weight: normal;\">on time</span>");
            }

            result << item;
        }
    }

    emit timetableResult(result);
}

void ParserXmlVasttrafikSe::parseSearchJourney(QNetworkReply *networkReply)
{
    qDebug() << "ParserXmlVasttrafikSe::parseSearchJourney(networkReply.url()=" << networkReply->url().toString() << ")";

    clearJourney();
    lastJourneyResultList = new JourneyResultList(this);

    /// Use fallback values for empty results (i.e. no connections found)
    lastJourneyResultList->setDepartureStation(m_searchJourneyParameters.departureStation.name);
    lastJourneyResultList->setViaStation(m_searchJourneyParameters.viaStation.name);
    lastJourneyResultList->setArrivalStation(m_searchJourneyParameters.arrivalStation.name);
    //: DATE, TIME
    lastJourneyResultList->setTimeInfo(tr("%1, %2", "DATE, TIME").arg(m_searchJourneyParameters.dateTime.date().toString(Qt::DefaultLocaleShortDate)).arg(m_searchJourneyParameters.dateTime.time().toString(Qt::DefaultLocaleShortDate)));

    m_earliestArrival = m_latestResultDeparture = QDateTime();

    QTextStream ts(networkReply->readAll());
    ts.setCodec("UTF-8");
    const QString xmlRawtext = ts.readAll();
    QDomDocument doc("result");
    if (doc.setContent(xmlRawtext, false)) {
        QDomNodeList tripNodeList = doc.elementsByTagName("Trip");
        for (unsigned int i = 0; i < tripNodeList.length(); ++i) {
            JourneyResultItem *jritem = new JourneyResultItem(lastJourneyResultList);
            JourneyDetailResultList *detailsList = new JourneyDetailResultList(this);

            /// Set default values for journey's start and end time
            QDateTime journeyStart = QDateTime::currentDateTime();
            QDateTime journeyEnd = QDateTime::currentDateTime();

            QDomNodeList legNodeList = tripNodeList.item(i).childNodes();
            int numStops = 0;
            QStringList trainTypes;
            int tripRtStatus = TRIP_RTDATA_NONE;
            for (unsigned int j = 0; j < legNodeList.length(); ++j) {
                QDomNode legNode = legNodeList.item(j);
                QDomNode originNode = legNode.namedItem("Origin");
                QDomNode destinationNode = legNode.namedItem("Destination");
                if (j == 0) {
                    journeyStart.setDate(QDate::fromString(getAttribute(originNode, "date"), QLatin1String("yyyy-MM-dd")));
                    journeyEnd.setDate(journeyStart.date());
                    const QTime time = QTime::fromString(getAttribute(originNode, "time"), "hh:mm");
                    journeyStart.setTime(time);
                    if (i == 0) {
                        const QDate date = QDate::fromString(getAttribute(originNode, "date"), QLatin1String("yyyy-MM-dd"));
                        lastJourneyResultList->setDepartureStation(getAttribute(originNode, "name"));
                        //: DATE, TIME
                        lastJourneyResultList->setTimeInfo(tr("%1, %2", "DATE, TIME").arg(date.toString(Qt::DefaultLocaleShortDate)).arg(time.toString(Qt::DefaultLocaleShortDate)));
                    }
                }
                if (j == legNodeList.length() - 1) {
                    journeyEnd.setTime(QTime::fromString(getAttribute(destinationNode, "time"), "hh:mm"));
                    if (i == 0)
                        lastJourneyResultList->setArrivalStation(getAttribute(destinationNode, "name"));
                }

                if (getAttribute(legNode, "type") != QLatin1String("WALK") || getAttribute(originNode, "name") != getAttribute(destinationNode, "name")) {
                    ++numStops;
                    trainTypes.append(i18nConnectionType(getAttribute(legNode, "name")));
                }

                JourneyDetailResultItem *jdrItem = new JourneyDetailResultItem(detailsList);
                jdrItem->setDepartureStation(getAttribute(originNode, "name"));
                const QString depTrack = getAttribute(originNode, "track");
                jdrItem->setDepartureInfo(depTrack.isEmpty() ? QChar(0x2014) : tr("Track %1").arg(depTrack));
                const QDateTime scheduledDepartureTime = QDateTime::fromString(getAttribute(originNode, "date") + getAttribute(originNode, "time"), "yyyy-MM-ddhh:mm");
                jdrItem->setDepartureDateTime(scheduledDepartureTime);
                jdrItem->setArrivalStation(getAttribute(destinationNode, "name"));
                const QString arrTrack = getAttribute(destinationNode, "track");
                jdrItem->setArrivalInfo(arrTrack.isEmpty() ? QChar(0x2014) : tr("Track %1").arg(arrTrack));
                const QDateTime scheduledArrivalTime = QDateTime::fromString(getAttribute(destinationNode, "date") + getAttribute(destinationNode, "time"), "yyyy-MM-ddhh:mm");
                jdrItem->setArrivalDateTime(scheduledArrivalTime);
                const QString direction = getAttribute(legNode, "direction");
                if (!direction.isEmpty())
                    jdrItem->setDirection(direction);
                if (getAttribute(legNode, "type") == QLatin1String("WALK"))
                    jdrItem->setTrain(tr("Walk"));
                else {
                    const QString connectionName = i18nConnectionType(getAttribute(legNode, "name"));
                    const QString fgColor = getAttribute(legNode, "fgColor");
                    const QString bgColor = getAttribute(legNode, "bgColor");
                    if (!fgColor.isEmpty() && !bgColor.isEmpty())
                        jdrItem->setTrain(QString(QLatin1String("<span style=\"color:%2; background-color: %3;\">%1</span>")).arg(connectionName).arg(fgColor).arg(bgColor));
                    else
                        jdrItem->setTrain(connectionName);
                }
                jdrItem->setInternalData1("NO setInternalData1");
                jdrItem->setInternalData2("NO setInternalData2");

                const QString realTimeDeparture = getAttribute(originNode, "rtTime");
                if (!realTimeDeparture.isEmpty()) {
                    const QTime realTimeTime = QTime::fromString(realTimeDeparture, QLatin1String("hh:mm"));
                    const int minutesTo = scheduledDepartureTime.time().msecsTo(realTimeTime) / 60000;
                    if (minutesTo > 3) {
                        jdrItem->setDepartureInfo(jdrItem->departureInfo() + tr("<br/><span style=\"color:#b30;\">%1 min late</span>").arg(minutesTo));
                        tripRtStatus = TRIP_RTDATA_WARNING;
                    } else {
                        if (tripRtStatus == TRIP_RTDATA_NONE) {
                            tripRtStatus = TRIP_RTDATA_ONTIME;
                        }
                        jdrItem->setDepartureInfo(jdrItem->departureInfo() + tr("<br/><span style=\"color:#093; font-weight: normal;\">on time</span>"));
                    }
                }

                const QString realTimeArrival = getAttribute(destinationNode, "rtTime");
                if (!realTimeArrival.isEmpty()) {
                    const QTime realTimeTime = QTime::fromString(realTimeArrival, QLatin1String("hh:mm"));
                    const int minutesTo = scheduledArrivalTime.time().msecsTo(realTimeTime) / 60000;
                    if (minutesTo > 3)
                        jdrItem->setArrivalInfo(jdrItem->arrivalInfo() + tr("<br/><span style=\"color:#b30;\">%1 min late</span>").arg(minutesTo));
                    else
                        jdrItem->setArrivalInfo(jdrItem->arrivalInfo() + tr("<br/><span style=\"color:#093; font-weight: normal;\">on time</span>"));
                }

                detailsList->appendItem(jdrItem);
            }

            if (journeyStart.time() > journeyEnd.time())
                journeyEnd = journeyEnd.addDays(1);

            jritem->setDate(journeyStart.date());
            const QString timeFormat = QLocale().timeFormat(QLocale::ShortFormat);
            jritem->setDepartureTime(journeyStart.time().toString(timeFormat));
            jritem->setArrivalTime(journeyEnd.time().toString(timeFormat));
            int diffTime = journeyStart.secsTo(journeyEnd);
            if (diffTime < 0) diffTime += 86400;
            jritem->setDuration(tr("%1:%2").arg(diffTime / 3600).arg(QString::number(diffTime / 60 % 60), 2, '0'));
            jritem->setTransfers(QString::number(legNodeList.length() - 1));
            trainTypes.removeDuplicates();
            jritem->setTrainType(trainTypes.join(", "));
            if (tripRtStatus == TRIP_RTDATA_WARNING)
                jritem->setMiscInfo(tr("<span style=\"color:#b30;\">traffic warning</span>"));
            else if (tripRtStatus == TRIP_RTDATA_ONTIME)
                jritem->setMiscInfo(tr("<span style=\"color:#093; font-weight: normal;\">on time</span>"));

            lastJourneyResultList->appendItem(jritem);

            const QString id = QString::number(i);
            jritem->setId(id);
            detailsList->setId(id);
            detailsList->setDepartureStation(lastJourneyResultList->departureStation());
            detailsList->setViaStation(lastJourneyResultList->viaStation());
            detailsList->setArrivalStation(lastJourneyResultList->arrivalStation());
            detailsList->setDuration(jritem->duration());
            detailsList->setArrivalDateTime(journeyEnd);
            detailsList->setDepartureDateTime(journeyStart);
            cachedJourneyDetails[id] = detailsList;

            if (!m_earliestArrival.isValid() || journeyEnd < m_earliestArrival)
                m_earliestArrival = journeyEnd.addSecs(-60);
            if (!m_latestResultDeparture.isValid() || journeyStart > m_latestResultDeparture)
                m_latestResultDeparture = journeyStart.addSecs(60);
        }
    }

    emit journeyResult(lastJourneyResultList);
}

void ParserXmlVasttrafikSe::searchJourneyLater()
{
    if (m_latestResultDeparture.isValid())
        searchJourney(m_searchJourneyParameters.departureStation, m_searchJourneyParameters.arrivalStation, m_searchJourneyParameters.viaStation, m_latestResultDeparture, Departure, 0);
    else {
        clearJourney();
        lastJourneyResultList = new JourneyResultList(this);
        lastJourneyResultList->setDepartureStation(m_searchJourneyParameters.departureStation.name);
        lastJourneyResultList->setViaStation(m_searchJourneyParameters.viaStation.name);
        lastJourneyResultList->setArrivalStation(m_searchJourneyParameters.arrivalStation.name);
        //: DATE, TIME
        lastJourneyResultList->setTimeInfo(tr("%1, %2", "DATE, TIME").arg(m_searchJourneyParameters.dateTime.date().toString(Qt::DefaultLocaleShortDate)).arg(m_searchJourneyParameters.dateTime.time().toString(Qt::DefaultLocaleShortDate)));
        emit journeyResult(lastJourneyResultList);
    }
}

void ParserXmlVasttrafikSe::searchJourneyEarlier()
{
    if (m_earliestArrival.isValid())
        searchJourney(m_searchJourneyParameters.departureStation, m_searchJourneyParameters.arrivalStation, m_searchJourneyParameters.viaStation, m_earliestArrival, Arrival, 0);
    else {
        clearJourney();
        lastJourneyResultList = new JourneyResultList(this);
        lastJourneyResultList->setDepartureStation(m_searchJourneyParameters.departureStation.name);
        lastJourneyResultList->setViaStation(m_searchJourneyParameters.viaStation.name);
        lastJourneyResultList->setArrivalStation(m_searchJourneyParameters.arrivalStation.name);
        //: DATE, TIME
        lastJourneyResultList->setTimeInfo(tr("%1, %2", "DATE, TIME").arg(m_searchJourneyParameters.dateTime.date().toString(Qt::DefaultLocaleShortDate)).arg(m_searchJourneyParameters.dateTime.time().toString(Qt::DefaultLocaleShortDate)));
        emit journeyResult(lastJourneyResultList);
    }
}

QString ParserXmlVasttrafikSe::i18nConnectionType(const QString &swedishText) const
{
    QString internalCopy = swedishText;
    return internalCopy.replace(QLatin1String("Buss"), tr("Bus")).replace(QLatin1String("Expbuss"), tr("Exp Bus")).replace(QLatin1String("EXPRESS"), QLatin1String("EXPR")).replace(QString::fromUtf8("Spårvagn"), tr("Tram")).replace(QString::fromUtf8("Färja"), tr("Ferry"));
}
