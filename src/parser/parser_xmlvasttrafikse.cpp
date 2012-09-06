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

QMap<QString, qulonglong> cachedStationNameToId;
QMap<qulonglong, qreal> cachedStationIdToLongitude;
QMap<qulonglong, qreal> cachedStationIdToLatitude;
QHash<QString, JourneyDetailResultList *> cachedJourneyDetails;

QString timeTableForStationStationName;
QString timeTableForStationDirectionStationName;
QDate timeTableForStationDate;
QTime timeTableForStationTime;
int timeTableForStationMode;
int timeTableForStationTrainrestrictions;

QString searchJourneyDepartureStation;
QString searchJourneyArrivalStation;
QString searchJourneyViaStation;
QDate searchJourneyDate;
QTime searchJourneyTime;
int searchJourneyMode;
int searchJourneyTrainrestrictions;


#define getAttribute(node, key) (node.attributes().namedItem(key).toAttr().value())

ParserXmlVasttrafikSe::ParserXmlVasttrafikSe(QObject *parent)
    : ParserAbstract(parent), apiKey(QLatin1String("47c5abaf-49d6-4c23-a1bd-b2e2766c4de7")), baseRestUrl(QLatin1String("http://api.vasttrafik.se/bin/rest.exe/v1/"))
{
    // nothing
}


void ParserXmlVasttrafikSe::getTimeTableForStation(QString stationName, QString directionStationName, QDate date, QTime time, int mode, int trainrestrictions)
{
    Q_UNUSED(directionStationName);
    Q_UNUSED(mode);
    Q_UNUSED(trainrestrictions);

    if (currentRequestState != FahrplanNS::noneRequest)
        return;
    currentRequestState = FahrplanNS::getTimeTableForStationRequest;

    timeTableForStationStationName = QString::null;
    qulonglong stationId = cachedStationNameToId.value(stationName, 0);

    if (stationId == 0) {
        timeTableForStationStationName = stationName;
        timeTableForStationDirectionStationName = directionStationName;
        timeTableForStationDate = date;
        timeTableForStationTime = time;
        timeTableForStationMode = mode;
        timeTableForStationTrainrestrictions = trainrestrictions;
        currentRequestState = FahrplanNS::noneRequest;
        findStationsByName(stationName);
        return;
    }

    QUrl url(baseRestUrl + QLatin1String("departureBoard"));
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

void ParserXmlVasttrafikSe::findStationsByName(QString stationName)
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

void ParserXmlVasttrafikSe::searchJourney(QString departureStation, QString arrivalStation, QString viaStation, QDate date, QTime time, int mode, int trainrestrictions)
{
    qDebug() << "ParserXmlVasttrafikSe::searchJourney(departureStation=" << departureStation << ", arrivalStation=" << arrivalStation << ", viaStation=" << viaStation << ", date=" << date.toString() << ", time=" << time.toString() << ", mode=" << mode << ", trainrestrictions=" << trainrestrictions << ")";

    if (currentRequestState != FahrplanNS::noneRequest)
        return;
    currentRequestState = FahrplanNS::searchJourneyRequest;

    searchJourneyDepartureStation = QString::null;

    qulonglong departureStationId = cachedStationNameToId.value(departureStation, 0);
    if (departureStationId == 0) {
        searchJourneyDepartureStation = departureStation;
        searchJourneyArrivalStation = arrivalStation;
        searchJourneyViaStation = viaStation;
        searchJourneyDate = date;
        searchJourneyTime = time;
        searchJourneyMode = mode;
        searchJourneyTrainrestrictions = trainrestrictions;
        currentRequestState = FahrplanNS::noneRequest;
        findStationsByName(departureStation);
        return;
    }

    qulonglong viaStationId = 0;
    if (!viaStation.isEmpty()) {
        viaStationId = cachedStationNameToId.value(viaStation, 0);
        if (viaStationId == 0) {
            searchJourneyDepartureStation = departureStation;
            searchJourneyArrivalStation = arrivalStation;
            searchJourneyViaStation = viaStation;
            searchJourneyDate = date;
            searchJourneyTime = time;
            searchJourneyMode = mode;
            searchJourneyTrainrestrictions = trainrestrictions;
            currentRequestState = FahrplanNS::noneRequest;
            findStationsByName(viaStation);
            return;
        }
    }

    qulonglong arrivalStationId = cachedStationNameToId.value(arrivalStation, 0);
    if (arrivalStationId == 0) {
        searchJourneyDepartureStation = departureStation;
        searchJourneyArrivalStation = arrivalStation;
        searchJourneyViaStation = viaStation;
        searchJourneyDate = date;
        searchJourneyTime = time;
        searchJourneyMode = mode;
        searchJourneyTrainrestrictions = trainrestrictions;
        currentRequestState = FahrplanNS::noneRequest;
        findStationsByName(arrivalStation);
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

void ParserXmlVasttrafikSe::getJourneyDetails(QString id)
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

            qulonglong id = getAttribute(locationNode, "id").toULongLong();
            cachedStationNameToId[stationName] = id;
            cachedStationIdToLongitude[id] = longitude;
            cachedStationIdToLatitude[id] = latitude;
        }
    }

    if (!timeTableForStationStationName.isEmpty()) {
        getTimeTableForStation(timeTableForStationStationName, timeTableForStationDirectionStationName, timeTableForStationDate, timeTableForStationTime, timeTableForStationMode, timeTableForStationTrainrestrictions);
    } else if (!searchJourneyDepartureStation.isEmpty()) {
        searchJourney(searchJourneyDepartureStation, searchJourneyArrivalStation, searchJourneyViaStation, searchJourneyDate, searchJourneyTime, searchJourneyMode, searchJourneyTrainrestrictions);
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
        QDomNodeList departureNodeList = doc.elementsByTagName("Departure");
        for (unsigned int i = 0; i < departureNodeList.length(); ++i) {
            QDomNode departureNode = departureNodeList.item(i);
            TimeTableResultItem *item = new TimeTableResultItem();

            const QString stationName = getAttribute(departureNode, "stop");
            qulonglong stationId = cachedStationNameToId.value(stationName, 0);
            item->setDestinationName(getAttribute(departureNode, "direction"));
            item->setPlatform(getAttribute(departureNode, "track"));
            item->setTrainType(i18nConnectionType(getAttribute(departureNode, "name")));
            item->setTime(QTime::fromString(getAttribute(departureNode, "time"), QLatin1String("hh:mm")));
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

    QTextStream ts(networkReply->readAll());
    ts.setCodec("UTF-8");
    const QString xmlRawtext = ts.readAll();
    QDomDocument doc("result");
    if (doc.setContent(xmlRawtext, false)) {
        QDomNodeList tripNodeList = doc.elementsByTagName("Trip");
        for (unsigned int i = 0; i < tripNodeList.length(); ++i) {
            JourneyResultItem *jritem = new JourneyResultItem();
            JourneyDetailResultList *detailsList = new JourneyDetailResultList();
            QDateTime journeyStart = QDateTime::currentDateTime();
            QDateTime journeyEnd = QDateTime::currentDateTime();

            QDomNodeList legNodeList = tripNodeList.item(i).childNodes();
            int numStops = 0;
            for (unsigned int j = 0; j < legNodeList.length(); ++j) {
                QDomNode legNode = legNodeList.item(j);
                QDomNode originNode = legNode.namedItem("Origin");
                QDomNode destinationNode = legNode.namedItem("Destination");
                if (j == 0) {
                    journeyStart.setDate(QDate::fromString(getAttribute(originNode, "date"), QLatin1String("yyyy-mm-dd")));
                    journeyEnd.setDate(journeyStart.date());
                    journeyStart.setTime(QTime::fromString(getAttribute(originNode, "time"), "hh:mm"));
                    if (i == 0)
                        journeyResultList->setDepartureStation(getAttribute(originNode, "name"));
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
                jdrItem->setDepartureDateTime(QDateTime::fromString(getAttribute(originNode, "date") + getAttribute(originNode, "time"), "yyyy-MM-ddhh:mm"));
                jdrItem->setArrivalStation(getAttribute(destinationNode, "name"));
                const QString arrTrack = getAttribute(destinationNode, "track");
                jdrItem->setArrivalInfo(arrTrack.isEmpty() ? QChar(0x2014) : tr("Track %1").arg(arrTrack));
                jdrItem->setArrivalDateTime(QDateTime::fromString(getAttribute(destinationNode, "date") + getAttribute(destinationNode, "time"), "yyyy-MM-ddhh:mm"));
                const QString direction = getAttribute(legNode, "direction");
                if (!direction.isEmpty())
                    jdrItem->setInfo(tr("to %1").arg(direction));
                if (getAttribute(legNode, "type") == QLatin1String("WALK"))
                    jdrItem->setTrain(tr("Walk"));
                else
                    jdrItem->setTrain(i18nConnectionType(getAttribute(legNode, "name")));
                jdrItem->setInternalData1("NO setInternalData1");
                jdrItem->setInternalData2("NO setInternalData2");
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
            detailsList->setArrivalStation(journeyResultList->arrivalStation());
            detailsList->setDuration(jritem->duration());
            detailsList->setArrivalDateTime(journeyStart);
            detailsList->setDepartureDateTime(journeyEnd);
            cachedJourneyDetails[id] = detailsList;
        }
    }

    emit journeyResult(journeyResultList);
}

QString ParserXmlVasttrafikSe::i18nConnectionType(const QString &swedishText) const
{
    QString internalCopy = swedishText;
    return internalCopy.replace(QLatin1String("Buss"), tr("Bus")).replace(QLatin1String("Expbuss"), tr("Exp Bus")).replace(QLatin1String("EXPRESS"), QLatin1String("EXPR")).replace(QString::fromUtf8("Spårvagn"), tr("Tram")).replace(QString::fromUtf8("Färja"), tr("Ferry"));
}