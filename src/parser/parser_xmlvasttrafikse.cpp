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

#include <QtXml/QDomDocument>

#include "parser_xmlvasttrafikse.h"

QMap<QString, qulonglong> cachedStationNameToId;
QMap<qulonglong, qreal> cachedStationIdToLongitude;
QMap<qulonglong, qreal> cachedStationIdToLatitude;

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

    if (currentRequestState != FahrplanNS::noneRequest) {
        return;
    }

    currentRequestState = FahrplanNS::getTimeTableForStationRequest;

    qWarning() << "getTimeTableForStation stationName=" << stationName;

    timeTableForStationStationName = QString::null;
    qulonglong stationId = cachedStationNameToId.value(stationName, 0);

    if (stationId == 0) {
		   qWarning() << "station" << stationName << "is not cached yet";
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

qWarning()<<"Requesting"<<url.toString();
      sendHttpRequest(url);

}

void ParserXmlVasttrafikSe::findStationsByName(QString stationName)
{
    if (currentRequestState != FahrplanNS::noneRequest) {
        return;
    }

    currentRequestState = FahrplanNS::stationsByNameRequest;

    qWarning() << "findStationsByName stationName=" << stationName;

    QUrl url(baseRestUrl + QLatin1String("location.name"));
    url.addQueryItem("authKey", apiKey);
    url.addQueryItem("format", "xml");
    url.addQueryItem("input", stationName);

	qWarning()<<"Requesting"<<url.toString();
    sendHttpRequest(url);
}

void ParserXmlVasttrafikSe::findStationsByCoordinates(qreal longitude, qreal latitude)
{
    if (currentRequestState != FahrplanNS::noneRequest) {
        return;
    }

    currentRequestState = FahrplanNS::stationsByCoordinatesRequest;

    qWarning() << "findStationsByCoordinates longitude=" << longitude << "  latitude=" << latitude;

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
    if (currentRequestState != FahrplanNS::noneRequest) {
        return;
    }

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
        qWarning() << "Requesting id for departure" << departureStation;
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
            qWarning() << "Requesting id for via" << viaStation;
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
        qWarning() << "Requesting id for arrival" << arrivalStation;
        currentRequestState = FahrplanNS::noneRequest;
        findStationsByName(arrivalStation);
        return;
    }

    qWarning() << "searchJourney from=" << departureStation<<departureStationId << " via=" << viaStation<<viaStationId << " to=" << arrivalStation<<arrivalStationId;

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

	qWarning()<<"Requesting"<<url.toString();
    sendHttpRequest(url);
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
    StationsResultList result;
    QTextStream ts(networkReply->readAll());
    ts.setCodec("UTF-8");
    const QString xmlRawtext = ts.readAll();

    QDomDocument doc("result");
    if (doc.setContent(xmlRawtext, false)) {
        QDomNodeList locationList = doc.elementsByTagName("StopLocation");
        for (unsigned int i = 0; i < locationList.length(); ++i) {
            QDomNamedNodeMap attributes = locationList.item(i).attributes();
            StationsResultItem *item = new StationsResultItem();
            const QString stationName = attributes.namedItem("name").toAttr().value();
            item->setStationName(stationName);
            qreal longitude = attributes.namedItem("lon").toAttr().value().toFloat();
            item->setLongitude(longitude);
            qreal latitude = attributes.namedItem("lat").toAttr().value().toFloat();
            item->setLatitude(latitude);

            result.appendItem(item);

            qulonglong id = attributes.namedItem("id").toAttr().value().toULongLong();
            cachedStationNameToId[stationName] = id;
            cachedStationIdToLongitude[id] = longitude;
            cachedStationIdToLatitude[id] = latitude;
        }
    }

    qWarning() << "got" << result.itemcount() << "stations";

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
    parseStationsByName(networkReply);
}

void ParserXmlVasttrafikSe::parseTimeTable(QNetworkReply *networkReply)
{
    TimeTableResultList *result = new TimeTableResultList();
    QTextStream ts(networkReply->readAll());
    ts.setCodec("UTF-8");
    const QString xmlRawtext = ts.readAll();

    QDomDocument doc("result");
    if (doc.setContent(xmlRawtext, false)) {
        QDomNodeList departureNodeList = doc.elementsByTagName("Departure");
        for (unsigned int i = 0; i < departureNodeList.length(); ++i) {
            QDomNamedNodeMap attributes = departureNodeList.item(i).attributes();
            TimeTableResultItem *item = new TimeTableResultItem();

            const QString stationName = attributes.namedItem("stop").toAttr().value();
            qulonglong stationId = cachedStationNameToId.value(stationName, 0);
            item->setDestinationName(attributes.namedItem("direction").toAttr().value());
            item->setPlatform(attributes.namedItem("track").toAttr().value());
            item->setTrainType(attributes.namedItem("name").toAttr().value());
            item->setTime(QTime::fromString(attributes.namedItem("time").toAttr().value(), QLatin1String("hh:mm")));
            if (stationId > 0) {
                item->setLongitude(cachedStationIdToLongitude[stationId]);
                item->setLatitude(cachedStationIdToLatitude[stationId]);
            }

            result->appendItem(item);
        }
    }

    qWarning() << "got" << result->itemcount() << "departures";
    emit timeTableResult(result);
}

void ParserXmlVasttrafikSe::parseSearchJourney(QNetworkReply *networkReply)
{
    JourneyResultList *journeyResultList = new JourneyResultList();
    QTextStream ts(networkReply->readAll());
    ts.setCodec("UTF-8");
    const QString xmlRawtext = ts.readAll();

    QDomDocument doc("result");
    if (doc.setContent(xmlRawtext, false)) {
        QDomNodeList tripNodeList = doc.elementsByTagName("Trip");
        for (unsigned int i = 0; i < tripNodeList.length(); ++i) {
            JourneyResultItem *jritem = new JourneyResultItem();
            QDateTime journeyStart = QDateTime::currentDateTime();
            QDateTime journeyEnd = QDateTime::currentDateTime();

            QDomNodeList legNodeList = tripNodeList.item(i).childNodes();
            for (unsigned int j = 0; j < legNodeList.length(); ++j) {
                QDomNode originNode = legNodeList.item(j).namedItem("Origin");
                QDomNode destinationNode = legNodeList.item(j).namedItem("Destination");
                QDomNamedNodeMap attributes = legNodeList.item(j).attributes();
                if (j == 0) {
                    journeyStart.setDate(QDate::fromString(originNode.attributes().namedItem("date").toAttr().value(), QLatin1String("yyyy-mm-dd")));
                    journeyEnd.setDate(journeyStart.date());
                    journeyStart.setTime(QTime::fromString(originNode.attributes().namedItem("time").toAttr().value(), "hh:mm"));
                    if (i == 0)
                        journeyResultList->setDepartureStation(originNode.attributes().namedItem("name").toAttr().value());
                }
                if (j == legNodeList.length() - 1) {
                    journeyEnd.setTime(QTime::fromString(destinationNode.attributes().namedItem("time").toAttr().value(), "hh:mm"));
                    if (i == 0)
                        journeyResultList->setArrivalStation(destinationNode.attributes().namedItem("name").toAttr().value());
                }

            }

            if (journeyStart.time() > journeyEnd.time())
                journeyEnd.addDays(1);
            jritem->setDate(journeyStart.date());
            jritem->setDepartureTime(journeyStart.time().toString());
            jritem->setArrivalTime(journeyEnd.time().toString());
            uint diffTime = journeyEnd.toTime_t() - journeyStart.toTime_t();
// FIXME            jritem->setDuration(journeyStart.toString()+" "+journeyEnd.toString());
			jritem->setDuration(QLatin1String("FIXME"));
            jritem->setTransfers(QString::number(legNodeList.length() - 1));
            journeyResultList->appendItem(jritem);
        }
    }

    qWarning() << "got" << journeyResultList->itemcount() << "journeys";
    emit journeyResult(journeyResultList);
}
