/*******************************************************************************

    This file is a part of Fahrplan for maemo 2009-2012
    Contributed by Thomas Fischer <fischer@unix-ag.uni-kl.de>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

*/

#include <QDebug>

#include <QtXml/QDomDocument>

#include "parser_xmlvasttrafikse.h"

const qlonglong ParserXmlVasttrafikSe::ERR_UNKNOWN_STATION = 0;
const qlonglong ParserXmlVasttrafikSe::ERR_INVALID_STATION = -1;


QMap<QString, qlonglong> cachedStationNameToId;
QMap<qlonglong, qreal> cachedStationIdToLongitude;
QMap<qlonglong, qreal> cachedStationIdToLatitude;
QHash<QString, JourneyDetailResultList *> cachedJourneyDetails;


#define getAttribute(node, key) (node.attributes().namedItem(key).toAttr().value())

ParserXmlVasttrafikSe::ParserXmlVasttrafikSe(QObject *parent)
    : ParserAbstract(parent), apiKey(QLatin1String("47c5abaf-49d6-4c23-a1bd-b2e2766c4de7")), baseRestUrl(QLatin1String("http://api.vasttrafik.se/bin/rest.exe/v1/"))
{
    m_searchJourneyParameters.isValid = false;
    m_timeTableForStationParameters.isValid = false;
}


void ParserXmlVasttrafikSe::getTimeTableForStation(const QString &stationName, const QString &directionStationName, const QDate &date, const QTime &time, Mode mode, int trainrestrictions)
{
    if (currentRequestState != FahrplanNS::noneRequest)
        return;
    currentRequestState = FahrplanNS::getTimeTableForStationRequest;

    m_timeTableForStationParameters.isValid = false;

    qlonglong stationId = cachedStationNameToId.value(stationName, ERR_UNKNOWN_STATION);

    if (stationId == ERR_UNKNOWN_STATION) {
        m_timeTableForStationParameters.isValid = true;
        m_timeTableForStationParameters.stationName = stationName;
        m_timeTableForStationParameters.directionStationName = directionStationName;
        m_timeTableForStationParameters.date = date;
        m_timeTableForStationParameters.time = time;
        m_timeTableForStationParameters.mode = mode;
        m_timeTableForStationParameters.trainrestrictions = trainrestrictions;

        currentRequestState = FahrplanNS::noneRequest;
        qDebug() << "Resolving id for station name" << stationName;
        findStationsByName(stationName);
        return;
    } else if (stationId == ERR_INVALID_STATION) {
        currentRequestState = FahrplanNS::noneRequest;
        emit timeTableResult(new TimeTableResultList());
        return;
    }

    QUrl url(baseRestUrl + (mode == Departure ? QLatin1String("departureBoard") : QLatin1String("arrivalBoard")));
    url.addQueryItem("authKey", apiKey);
    url.addQueryItem("format", "xml");
    url.addQueryItem("date", date.toString("yyyy-MM-dd"));
    url.addQueryItem("time", time.toString("hh:mm"));
    url.addQueryItem("id", QString::number(stationId));
    url.addQueryItem("useVas", "1");
    url.addQueryItem("useLDTrain", "0");
    url.addQueryItem("useRegTrain", "1");
    url.addQueryItem("useBus", "1");
    url.addQueryItem("useBoat", "1");
    url.addQueryItem("useTram", "1");
    url.addQueryItem("excludeDR", "1");

    sendHttpRequest(url);
}

void ParserXmlVasttrafikSe::findStationsByName(const QString &stationName)
{
    qDebug() << "ParserXmlVasttrafikSe::findStationsByName(stationName" << stationName << ")";

    if (currentRequestState != FahrplanNS::noneRequest)
        return;
    currentRequestState = FahrplanNS::stationsByNameRequest;

    QUrl url(baseRestUrl + QLatin1String("location.name"));
    url.addQueryItem("authKey", apiKey);
    url.addQueryItem("format", "xml");
    url.addQueryItem("input", stationName);

    sendHttpRequest(url);
}

void ParserXmlVasttrafikSe::findStationsByCoordinates(qreal longitude, qreal latitude)
{
    qDebug() << "ParserXmlVasttrafikSe::findStationsByCoordinates(longitude=" << longitude << ", latitude=" << latitude << ")";

    if (currentRequestState != FahrplanNS::noneRequest)
        return;
    currentRequestState = FahrplanNS::stationsByCoordinatesRequest;

    QUrl url(baseRestUrl + QLatin1String("location.nearbystops"));
    url.addQueryItem("authKey", apiKey);
    url.addQueryItem("format", "xml");
    url.addQueryItem("originCoordLat", QString::number(latitude));
    url.addQueryItem("originCoordLong", QString::number(longitude));
    url.addQueryItem("maxNo", QString::number(5));

    sendHttpRequest(url);
}

void ParserXmlVasttrafikSe::searchJourney(const QString &departureStation, const QString &arrivalStation, const QString &viaStation, const QDate &date, const QTime &time, Mode mode, int trainrestrictions)
{
    qDebug() << "ParserXmlVasttrafikSe::searchJourney(departureStation=" << departureStation << ", arrivalStation=" << arrivalStation << ", viaStation=" << viaStation << ", date=" << date.toString() << ", time=" << time.toString() << ", mode=" << mode << ", trainrestrictions=" << trainrestrictions << ")";

    if (currentRequestState != FahrplanNS::noneRequest)
        return;
    currentRequestState = FahrplanNS::searchJourneyRequest;

    m_searchJourneyParameters.isValid = false;
    m_searchJourneyParameters.departureStation = departureStation;
    m_searchJourneyParameters.arrivalStation = arrivalStation;
    m_searchJourneyParameters.viaStation = viaStation;
    m_searchJourneyParameters.date = date;
    m_searchJourneyParameters.time = time;
    m_searchJourneyParameters.mode = mode;
    m_searchJourneyParameters.trainrestrictions = trainrestrictions;

    qlonglong departureStationId = cachedStationNameToId.value(departureStation, ERR_UNKNOWN_STATION);
    if (departureStationId == ERR_UNKNOWN_STATION) {
        m_searchJourneyParameters.isValid = true;
        currentRequestState = FahrplanNS::noneRequest;
        qDebug() << "Resolving id for departure station" << departureStation;
        findStationsByName(departureStation);
        return;
    } else if (departureStationId == ERR_INVALID_STATION) {
        currentRequestState = FahrplanNS::noneRequest;
        JourneyResultList *jrl = new JourneyResultList();
        jrl->setTimeInfo(tr("Invalid departure station"));
        jrl->setDepartureStation(departureStation);
        jrl->setViaStation(viaStation);
        jrl->setArrivalStation(arrivalStation);
        emit journeyResult(jrl);
        return;
    }

    qlonglong viaStationId = ERR_INVALID_STATION;
    if (!viaStation.isEmpty()) {
        viaStationId = cachedStationNameToId.value(viaStation, ERR_UNKNOWN_STATION);
        if (viaStationId == ERR_UNKNOWN_STATION) {
            m_searchJourneyParameters.isValid = true;
            currentRequestState = FahrplanNS::noneRequest;
            qDebug() << "Resolving id for via station" << viaStation;
            findStationsByName(viaStation);
            return;
        } else if (viaStationId == ERR_INVALID_STATION) {
            currentRequestState = FahrplanNS::noneRequest;
            JourneyResultList *jrl = new JourneyResultList();
            jrl->setTimeInfo(tr("Invalid via station"));
            jrl->setDepartureStation(departureStation);
            jrl->setViaStation(viaStation);
            jrl->setArrivalStation(arrivalStation);
            emit journeyResult(jrl);
            return;
        }
    }

    qlonglong arrivalStationId = cachedStationNameToId.value(arrivalStation, ERR_UNKNOWN_STATION);
    if (arrivalStationId == ERR_UNKNOWN_STATION) {
        m_searchJourneyParameters.isValid = true;
        currentRequestState = FahrplanNS::noneRequest;
        qDebug() << "Resolving id for arrival station" << arrivalStation;
        findStationsByName(arrivalStation);
        return;
    } else if (arrivalStationId == ERR_INVALID_STATION) {
        currentRequestState = FahrplanNS::noneRequest;
        JourneyResultList *jrl = new JourneyResultList();
        jrl->setTimeInfo(tr("Invalid arrival station"));
        jrl->setDepartureStation(departureStation);
        jrl->setViaStation(viaStation);
        jrl->setArrivalStation(arrivalStation);
        emit journeyResult(jrl);
        return;
    }

    QUrl url(baseRestUrl + QLatin1String("trip"));
    url.addQueryItem("authKey", apiKey);
    url.addQueryItem("format", "xml");
    url.addQueryItem("date", date.toString("yyyy-MM-dd"));
    url.addQueryItem("time", time.toString("hh:mm"));
    url.addQueryItem("originId", QString::number(departureStationId));
    if (viaStationId > 0)
        url.addQueryItem("viaId", QString::number(viaStationId));
    if (mode == Arrival)
        url.addQueryItem("searchForArrival", "yes");
    url.addQueryItem("destId", QString::number(arrivalStationId));
    url.addQueryItem("useVas", "1");
    url.addQueryItem("useLDTrain", "0");
    url.addQueryItem("useRegTrain", "1");
    url.addQueryItem("useBus", "1");
    url.addQueryItem("useBoat", "1");
    url.addQueryItem("useTram", "1");
    url.addQueryItem("excludeDR", "1");
    url.addQueryItem("numTrips", "5");

    sendHttpRequest(url);
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

    StationsResultList result;
    QTextStream ts(networkReply->readAll());
    ts.setCodec("UTF-8");
    const QString xmlRawtext = ts.readAll();

    const QString stationNameFromQueryString = networkReply->url().queryItemValue(QLatin1String("input"));
    if (!stationNameFromQueryString.isEmpty()) {
        /// just in case the station does not exist for this provider
        cachedStationNameToId[stationNameFromQueryString] = ERR_INVALID_STATION;
    }

    QDomDocument doc("result");
    if (doc.setContent(xmlRawtext, false)) {
        QDomNodeList locationList = doc.elementsByTagName("StopLocation");
        for (unsigned int i = 0; i < locationList.length(); ++i) {
            QDomNode locationNode = locationList.item(i);
            StationsResultItem *item = new StationsResultItem();
            const QString stationName = getAttribute(locationNode, "name");
            item->setStationName(stationName);
            qreal longitude = getAttribute(locationNode, "lon").toFloat();
            item->setLongitude(longitude);
            qreal latitude = getAttribute(locationNode, "lat").toFloat();
            item->setLatitude(latitude);

            result.appendItem(item);

            qlonglong id = getAttribute(locationNode, "id").toLongLong();
            cachedStationNameToId[stationName] = id;
            cachedStationIdToLongitude[id] = longitude;
            cachedStationIdToLatitude[id] = latitude;
        }
    }

    if (m_timeTableForStationParameters.isValid) {
        getTimeTableForStation(m_timeTableForStationParameters.stationName, m_timeTableForStationParameters.directionStationName, m_timeTableForStationParameters.date, m_timeTableForStationParameters.time, m_timeTableForStationParameters.mode, m_timeTableForStationParameters.trainrestrictions);
    } else if (m_searchJourneyParameters.isValid) {
        searchJourney(m_searchJourneyParameters.departureStation, m_searchJourneyParameters.arrivalStation, m_searchJourneyParameters.viaStation, m_searchJourneyParameters.date, m_searchJourneyParameters.time, m_searchJourneyParameters.mode, m_searchJourneyParameters.trainrestrictions);
    } else {
        emit stationsResult(&result);
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

    TimeTableResultList *result = new TimeTableResultList();
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
            TimeTableResultItem *item = new TimeTableResultItem();

            const QString stationName = getAttribute(node, "stop");
            qlonglong stationId = cachedStationNameToId.value(stationName, ERR_UNKNOWN_STATION);
            item->setDestinationName(getAttribute(node, isArrival ? "origin" : "direction"));
            item->setPlatform(getAttribute(node, "track"));
            const QString connectionName = i18nConnectionType(getAttribute(node, "name"));
            const QString fgColor = getAttribute(node, "fgColor");
            const QString bgColor = getAttribute(node, "bgColor");
            if (!fgColor.isEmpty() && !bgColor.isEmpty())
                item->setTrainType(QString(QLatin1String("<span style=\"color:%2; background-color: %3;\">%1</span>")).arg(connectionName).arg(fgColor).arg(bgColor));
            else
                item->setTrainType(connectionName);
            const QTime scheduledTime = QTime::fromString(getAttribute(node, "time"), QLatin1String("hh:mm"));
            item->setTime(scheduledTime);
            const QString realTimeStr = getAttribute(node, "rtTime");
            if (!realTimeStr.isEmpty()) {
                const QTime realTimeTime = QTime::fromString(realTimeStr, QLatin1String("hh:mm"));
                const int minutesTo = scheduledTime.msecsTo(realTimeTime) / 60000;
                if (minutesTo > 3)
                    item->setMiscInfo(tr("<span style=\"color:#b30;\">%1 min late</span>").arg(minutesTo));
                else
                    item->setMiscInfo(tr("<span style=\"color:#093; font-weight: normal;\">on time</span>"));
            }
            if (stationId > 0) {
                item->setLongitude(cachedStationIdToLongitude[stationId]);
                item->setLatitude(cachedStationIdToLatitude[stationId]);
            }

            result->appendItem(item);
        }
    }

    emit timeTableResult(result);
}

void ParserXmlVasttrafikSe::parseSearchJourney(QNetworkReply *networkReply)
{
    qDebug() << "ParserXmlVasttrafikSe::parseSearchJourney(networkReply.url()=" << networkReply->url().toString() << ")";

    JourneyResultList *journeyResultList = new JourneyResultList();

    for (QHash<QString, JourneyDetailResultList *>::Iterator it = cachedJourneyDetails.begin(); it != cachedJourneyDetails.end();) {
        JourneyDetailResultList *jdrl = it.value();
        it = cachedJourneyDetails.erase(it);
        delete jdrl;
    }

    /// Use fallback values for empty results (i.e. no connections found)
    journeyResultList->setDepartureStation(m_searchJourneyParameters.departureStation);
    journeyResultList->setViaStation(m_searchJourneyParameters.viaStation);
    journeyResultList->setArrivalStation(m_searchJourneyParameters.arrivalStation);
    journeyResultList->setTimeInfo(tr("%1, %2", "DATE, TIME").arg(m_searchJourneyParameters.date.toString(Qt::DefaultLocaleShortDate)).arg(m_searchJourneyParameters.time.toString(Qt::DefaultLocaleShortDate)));

    m_earliestResultTime = m_latestResultTime = QTime();

    QTextStream ts(networkReply->readAll());
    ts.setCodec("UTF-8");
    const QString xmlRawtext = ts.readAll();
    QDomDocument doc("result");
    if (doc.setContent(xmlRawtext, false)) {
        QDomNodeList tripNodeList = doc.elementsByTagName("Trip");
        for (unsigned int i = 0; i < tripNodeList.length(); ++i) {
            JourneyResultItem *jritem = new JourneyResultItem();
            JourneyDetailResultList *detailsList = new JourneyDetailResultList();

            /// Set default values for journey's start and end time
            QDateTime journeyStart = QDateTime::currentDateTime();
            QDateTime journeyEnd = QDateTime::currentDateTime();

            QDomNodeList legNodeList = tripNodeList.item(i).childNodes();
            int numStops = 0;
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
                        journeyResultList->setDepartureStation(getAttribute(originNode, "name"));
                        journeyResultList->setTimeInfo(tr("%1, %2", "DATE, TIME").arg(date.toString(Qt::DefaultLocaleShortDate)).arg(time.toString(Qt::DefaultLocaleShortDate)));
                    }
                }
                if (j == legNodeList.length() - 1) {
                    journeyEnd.setTime(QTime::fromString(getAttribute(destinationNode, "time"), "hh:mm"));
                    if (i == 0)
                        journeyResultList->setArrivalStation(getAttribute(destinationNode, "name"));
                }

                if (getAttribute(legNode, "type") != QLatin1String("WALK") || getAttribute(originNode, "name") != getAttribute(destinationNode, "name"))
                    ++numStops;

                JourneyDetailResultItem *jdrItem = new JourneyDetailResultItem();
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
                    jdrItem->setInfo(tr("to %1").arg(direction));
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
                    if (minutesTo > 3)
                        jdrItem->setDepartureInfo(jdrItem->departureInfo() + tr("<br/><span style=\"color:#b30;\">%1 min late</span>").arg(minutesTo));
                    else
                        jdrItem->setDepartureInfo(jdrItem->departureInfo() + tr("<br/><span style=\"color:#093; font-weight: normal;\">on time</span>"));
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
                journeyEnd.addDays(1);
            jritem->setDate(journeyStart.date());
            jritem->setDepartureTime(journeyStart.time().toString("hh:mm"));
            jritem->setArrivalTime(journeyEnd.time().toString("hh:mm"));
            int diffTime = journeyStart.secsTo(journeyEnd);
            if (diffTime < 0) diffTime += 86400;
            jritem->setDuration(tr("%1:%2").arg(diffTime / 3600).arg(QString::number(diffTime / 60 % 60), 2, '0'));
            jritem->setTransfers(QString::number(legNodeList.length() - 1));
            journeyResultList->appendItem(jritem);

            const QString id = QString::number(i);
            jritem->setId(id);
            detailsList->setId(id);
            detailsList->setDepartureStation(journeyResultList->departureStation());
            detailsList->setViaStation(journeyResultList->viaStation());
            detailsList->setArrivalStation(journeyResultList->arrivalStation());
            detailsList->setDuration(jritem->duration());
            detailsList->setArrivalDateTime(journeyStart);
            detailsList->setDepartureDateTime(journeyEnd);
            cachedJourneyDetails[id] = detailsList;

            if (!m_earliestResultTime.isValid() || journeyStart.time() < m_earliestResultTime)
                m_earliestResultTime = journeyStart.time().addSecs(-60);
            if (!m_latestResultTime.isValid() || journeyStart.time() > m_latestResultTime)
                m_latestResultTime = journeyStart.time().addSecs(60);
        }
    }

    emit journeyResult(journeyResultList);
}

void ParserXmlVasttrafikSe::searchJourneyLater()
{
    if (m_latestResultTime.isValid())
        searchJourney(m_searchJourneyParameters.departureStation, m_searchJourneyParameters.arrivalStation, m_searchJourneyParameters.viaStation, m_searchJourneyParameters.date, m_latestResultTime, Departure, 0);
}

void ParserXmlVasttrafikSe::searchJourneyEarlier()
{
    if (m_earliestResultTime.isValid())
        searchJourney(m_searchJourneyParameters.departureStation, m_searchJourneyParameters.arrivalStation, m_searchJourneyParameters.viaStation, m_searchJourneyParameters.date, m_earliestResultTime, Arrival, 0);
}

QString ParserXmlVasttrafikSe::i18nConnectionType(const QString &swedishText) const
{
    QString internalCopy = swedishText;
    return internalCopy.replace(QLatin1String("Buss"), tr("Bus")).replace(QLatin1String("Expbuss"), tr("Exp Bus")).replace(QLatin1String("EXPRESS"), QLatin1String("EXPR")).replace(QString::fromUtf8("Spårvagn"), tr("Tram")).replace(QString::fromUtf8("Färja"), tr("Ferry"));
}