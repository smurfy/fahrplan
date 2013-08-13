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

#include "parser_hafasxml.h"

#include <QBuffer>
#include <QNetworkReply>
#include <QXmlStreamReader>

#if defined(BUILD_FOR_QT5)
  #include <QUrlQuery>
#endif

ParserHafasXml::ParserHafasXml(QObject *parent) :
    ParserAbstract(parent)
{
     //baseUrl = "http://fahrplan.oebb.at/bin/query.exe"; //OEB (fully operational/no RT) //no xmlhandle, detaildate already present!
     //baseUrl = "http://hafas.bene-system.com/bin/query.exe"; //hafas dev?? system? / no gps
     //baseUrl = "http://reiseauskunft.bahn.de/bin/query.exe"; //bahn.de (journey stuff fails)
     //baseUrl = "http://fahrplan.sbb.ch/bin/query.exe"; //SBB (only returns one journey) / Xmlhandle present
     //baseUrl = "http://www.fahrplaner.de/hafas/query.exe"; //?? No Gps, returns only one result
     //baseUrl = "http://www.rejseplanen.dk/bin/query.exe";//?? No Gps, returns only one result //no xmlhandle, detaildate already present!
     //baseUrl = "http://airs1.septa.org/bin/query.exe";// not working at all
     //baseUrl = "http://mobiliteitszentral.hafas.de/hafas/query.exe";// Luxenburg, //no xmlhandle, detaildate already present!

     hafasHeader.accessid = "";
     hafasHeader.prod = "String";
     hafasHeader.ver = "1.1";

     STTableMode = 0;
}

bool ParserHafasXml::supportsGps()
{
    return true;
}

bool ParserHafasXml::supportsVia()
{
    return true;
}

bool ParserHafasXml::supportsTimeTable()
{
    return true;
}

bool ParserHafasXml::supportsTimeTableDirection()
{
    return STTableMode == 0;
}

void ParserHafasXml::getTimeTableForStation(const Station &currentStation, const Station &directionStation, const QDateTime &dateTime, ParserAbstract::Mode mode, int trainrestrictions)
{
    if (currentRequestState != FahrplanNS::noneRequest) {
        return;
    }

    currentRequestState = FahrplanNS::getTimeTableForStationRequest;

    if (STTableMode == 0) {
        currentRequestState = FahrplanNS::getTimeTableForStationRequest;

        QString trainrestr = getTrainRestrictionsCodes(trainrestrictions);

        QByteArray postData = "";
        postData.append("<?xml version=\"1.0\" encoding=\"UTF-8\" ?><ReqC accessId=\"" + hafasHeader.accessid + "\" ver=\"" + hafasHeader.ver + "\" prod=\"" + hafasHeader.prod + "\" lang=\"EN\">");
        postData.append("<STBReq boardType=\"");
        if (mode == Departure) {
            postData.append("DEP");
        } else /* (getTimeTableForStationRequestData.mode == Arrival) */ {
            postData.append("ARR");
        }
        postData.append("\" maxJourneys=\"40\">");
        postData.append("<Time>");
        postData.append(dateTime.toString("hh:mm"));
        postData.append("</Time>");
        postData.append("<Period><DateBegin><Date>");
        postData.append(dateTime.toString("yyyyMMdd"));
        postData.append("</Date></DateBegin><DateEnd><Date>");
        postData.append(dateTime.toString("yyyyMMdd"));
        postData.append("</Date></DateEnd></Period>");
        postData.append("<TableStation externalId=\"");
        postData.append(parseExternalIds(currentStation.id));
        postData.append("\"></TableStation>");
        postData.append("<ProductFilter>");
        postData.append(trainrestr);
        postData.append("</ProductFilter>");
        postData.append("<DirectionFilter externalId=\"");
        postData.append(parseExternalIds(directionStation.id));
        postData.append("\"></DirectionFilter>");
        postData.append("</STBReq>");
        postData.append("</ReqC>");

        qDebug() << postData;

        sendHttpRequest(QUrl(baseXmlUrl), postData);
    }

    if (STTableMode == 1) {
        QString trainrestr = getTrainRestrictionsCodes(trainrestrictions);
        QUrl uri(baseSTTableUrl);
#if defined(BUILD_FOR_QT5)
        QUrlQuery query;
#else
        QUrl query;
#endif
        query.addQueryItem("productsFilter", trainrestr);
        if (mode == Departure) {
            query.addQueryItem("boardType", "dep");
        } else /* (getTimeTableForStationRequestData.mode == Arrival) */ {
            query.addQueryItem("boardType", "arr");
        }
        query.addQueryItem("date", dateTime.toString("dd.MM.yyyy"));
        query.addQueryItem("time", dateTime.toString("hh:mm"));
        query.addQueryItem("input", currentStation.name);
        query.addQueryItem("REQ0JourneyStopsSID", currentStation.id.toString());
        query.addQueryItem("maxJourneys", "50");
        query.addQueryItem("start", "yes");
        query.addQueryItem("L", "vs_java3");
#if defined(BUILD_FOR_QT5)
        uri.setQuery(query);
#else
        uri.setQueryItems(query.queryItems());
#endif
        sendHttpRequest(uri);
    }
}

void ParserHafasXml::parseTimeTable(QNetworkReply *networkReply)
{
    if (STTableMode == 0) {
        parseTimeTableMode0(networkReply);
    }

    if (STTableMode == 1) {
        parseTimeTableMode1(networkReply);
    }
}

void ParserHafasXml::parseTimeTableMode1(QNetworkReply *networkReply)
{
    TimeTableResultList *result = new TimeTableResultList();

    QString data = QString::fromLatin1(networkReply->readAll());

    //Add a root element, because its sometimes missing
    if (data.indexOf("StationTable") == -1) {
        data.prepend("<StationTable>");
        data.append("</StationTable>");
    }

    QXmlStreamReader xml;
    xml.addData(data);

    while (!xml.atEnd()) {
        xml.readNext();

        if (xml.isStartElement() && (xml.name() == "Err")) {
            QString errorMsg = xml.attributes().value("text").toString().simplified();
            emit errorOccured(tr("Backend returns an error: ") + errorMsg);
            qWarning()<<"ParserHafasXml::parseTimeTableMode1: "<<errorMsg;
        }

        if (xml.isStartElement() && (xml.name() == "Journey")) {
            TimeTableResultItem *item = new TimeTableResultItem();

            QString dest = xml.attributes().value("dir").toString().simplified();
            QString station = xml.attributes().value("depStation").toString().simplified();
            QString train = xml.attributes().value("hafasname").toString().simplified();

            if (dest.isEmpty()) {
                dest = xml.attributes().value("targetLoc").toString().simplified();
            }
            if (train.isEmpty()) {
                train = xml.attributes().value("prod").toString().simplified();

                if (train.indexOf("#")) {
                    train = train.left(train.indexOf("#"));
                }
            }

            //get delay infos,
            QString txtDelay = xml.attributes().value("delay").toString().simplified();
            //QString intDelay = xml.attributes().value("e_delay").toString().simplified();
            QString reasonDelay = xml.attributes().value("delayReason").toString().simplified();
            QString miscInfo = "";

            if (!txtDelay.isEmpty()) {
                if (txtDelay == "-") {
                    miscInfo = "";
                } else if (txtDelay == "0") {
                    miscInfo = tr("On-Time");
                } else {
                    miscInfo = txtDelay;
                }
            }

            if (!reasonDelay.isEmpty()) {
                if (!miscInfo.isEmpty()) {
                    miscInfo.append(": ");
                }
                miscInfo.append(reasonDelay);
            }

            item->setDestinationName(dest);
            item->setStationName(station);
            item->setPlatform(xml.attributes().value("platform").toString().simplified());
            item->setTrainType(train);
            item->setTime(QTime::fromString(xml.attributes().value("fpTime").toString(), "hh:mm"));
            item->setMiscInfo(miscInfo);

            result->appendItem(item);
        }

    }
    emit timeTableResult(result);
}

void ParserHafasXml::parseTimeTableMode0(QNetworkReply *networkReply)
{
    TimeTableResultList *result = new TimeTableResultList();

    QString data = QString::fromUtf8(networkReply->readAll());

    QXmlStreamReader xml;
    xml.addData(data);

    while (!xml.atEnd()) {
        xml.readNext();
        if (xml.isStartElement() && (xml.name() == "STBJourney")) {
            TimeTableResultItem *item = new TimeTableResultItem();

            while (!xml.atEnd()) {
                xml.readNext();
                if (xml.isStartElement() && xml.name() == "Station") {
                    item->setStationName(xml.attributes().value("name").toString().simplified());
                    item->setLongitude(xml.attributes().value("x").toString().toInt());
                    item->setLatitude(xml.attributes().value("y").toString().toInt());
                }

                if (xml.isStartElement() && (xml.name() == "Dep" || xml.name() == "Arr" )) {
                    while (!xml.atEnd()) {
                        xml.readNext();
                        if (xml.isStartElement() && xml.name() == "Time") {
                            xml.readNext();
                            item->setTime(QTime::fromString(xml.text().toString(), "hh:mm"));
                        }

                        if (xml.isStartElement() && xml.name() == "Platform") {
                            while (!xml.atEnd()) {
                                xml.readNext();
                                if (xml.isStartElement() && xml.name() == "Text") {
                                    xml.readNext();
                                    item->setPlatform(xml.text().toString().simplified());
                                }

                                if (xml.isEndElement() && xml.name() == "Platform") {
                                   break;
                                }
                            }
                        }

                        if (xml.isEndElement() && (xml.name() == "Dep" || xml.name() == "Arr" )) {
                           break;
                        }
                    }
                }

                if (xml.isStartElement() && xml.name() == "Attribute") {
                    QString currentAttributeType = xml.attributes().value("type").toString();
                    while (!xml.atEnd()) {
                        xml.readNext();
                        if (xml.isStartElement() && xml.name() == "Text") {
                            xml.readNext();

                            if (currentAttributeType == "DIRECTION") {
                                item->setDestinationName(xml.text().toString().simplified());
                            }
                            if (currentAttributeType == "NAME") {
                                item->setTrainType(xml.text().toString().simplified());
                            }
                        }

                        if (xml.isEndElement() && xml.name() == "Attribute") {
                           break;
                        }
                    }
                }
                if (xml.isEndElement() && xml.name() == "STBJourney") {
                    result->appendItem(item);
                    break;
                }
            }
        }
    }

    emit timeTableResult(result);
}

void ParserHafasXml::findStationsByName(const QString &stationName)
{
    if (currentRequestState != FahrplanNS::noneRequest) {
        return;
    }

    currentRequestState = FahrplanNS::stationsByNameRequest;
    QString internalStationName = stationName;
    internalStationName.replace("\"", "");

    QByteArray postData = "";
    postData.append("<?xml version=\"1.0\" encoding=\"UTF-8\" ?><ReqC accessId=\"" + hafasHeader.accessid + "\" ver=\"" + hafasHeader.ver + "\" prod=\"" + hafasHeader.prod + "\" lang=\"EN\"><MLcReq><MLc n=\"");
    postData.append(internalStationName);
    postData.append("\" t=\"ST\" /></MLcReq></ReqC>");

    sendHttpRequest(QUrl(baseXmlUrl), postData);
}

void ParserHafasXml::findStationsByCoordinates(qreal longitude, qreal latitude)
{
    if (currentRequestState != FahrplanNS::noneRequest) {
        return;
    }

    currentRequestState = FahrplanNS::stationsByCoordinatesRequest;

    //We must format the lat and longitude to have the ??.?????? format.
    QString zeros      = "0";
    QString sLongitude = QString::number(longitude).append(zeros.repeated(6));
    QString sLatitude  = QString::number(latitude).append(zeros.repeated(6));

    QRegExp regexp = QRegExp("(\\d*)\\.(\\d\\d\\d\\d\\d\\d)(\\d*)");
    regexp.setMinimal(true);

    regexp.indexIn(sLongitude);
    sLongitude = regexp.cap(1) + regexp.cap(2);

    regexp.indexIn(sLatitude);
    sLatitude = regexp.cap(1) + regexp.cap(2);

    QUrl uri(baseUrl + "/eol");
#if defined(BUILD_FOR_QT5)
    QUrlQuery query;
#else
    QUrl query;
#endif
    query.addQueryItem("look_x", sLongitude);
    query.addQueryItem("look_y", sLatitude);
    query.addQueryItem("performLocating", "2");
    query.addQueryItem("tpl", "stopsnear");
    query.addQueryItem("L", "vs_java");
    query.addQueryItem("look_maxdist", "5000");
    query.addQueryItem("look_maxno", "40");
#if defined(BUILD_FOR_QT5)
    uri.setQuery(query);
#else
    uri.setQueryItems(query.queryItems());
#endif
    sendHttpRequest(uri);
}

void ParserHafasXml::parseStationsByName(QNetworkReply *networkReply)
{
    QString data = QString::fromLatin1(networkReply->readAll());
    StationsResultList *result = internalParseStationsByName(data);
    emit stationsResult(result);
}

StationsResultList* ParserHafasXml::internalParseStationsByName(const QString &data)
{
    StationsResultList *result = new StationsResultList();

    QXmlStreamReader xml;
    xml.addData(data);

    while (!xml.atEnd()) {
        xml.readNext();
        if (xml.isStartElement()) {
            if (xml.name() == "MLc")
            {
                StationsResultItem *item = new StationsResultItem();
                item->setStationName(xml.attributes().value("n").toString());
                item->setStationType(xml.attributes().value("t").toString());
                item->setStationId(xml.attributes().value("i").toString());
                item->setLongitude(xml.attributes().value("x").toString().toInt());
                item->setLatitude(xml.attributes().value("y").toString().toInt());
                QString miscInfo = xml.attributes().value("dist").toString();
                if (!miscInfo.isEmpty()) {
                    item->setMiscInfo(miscInfo + "m");
                }
                result->appendItem(item);
            }
        }
    }
    if (xml.error() && xml.error() != QXmlStreamReader::PrematureEndOfDocumentError) {
        qWarning() << "XML ERROR:" << xml.lineNumber() << ": " << xml.errorString();
    }

    return result;
}


void ParserHafasXml::parseStationsByCoordinates(QNetworkReply *networkReply)
{
    //Normally hafas returns the data as Latin1, but here we get utf8.
    QString data = QString::fromUtf8(networkReply->readAll());
    StationsResultList *result = internalParseStationsByName(data);
    emit stationsResult(result);
}

QString ParserHafasXml::getTrainRestrictionsCodes(int trainrestrictions)
{
    QString trainrestr = "1111111111111111";
    if (trainrestrictions == 0) {
        trainrestr = "1111111111111111"; //ALL
    } else if (trainrestrictions == 1) {
        trainrestr = "0111111111000000"; //All without ICE
    } else if (trainrestrictions == 2) {
        trainrestr = "00011111110000000"; //Only local transport
    } else if (trainrestrictions == 3) {
        trainrestr = "0001011111000000"; //Only local transport without sbahn
    }

    return trainrestr;
}

QStringList ParserHafasXml::getTrainRestrictions()
{
    QStringList result;
    result.append(tr("All"));
    result.append(tr("All without ICE"));
    result.append(tr("Only local transport"));
    result.append(tr("Local transport without S-Bahn"));
    return result;
}

void ParserHafasXml::searchJourney(const Station &departureStation, const Station &viaStation, const Station &arrivalStation, const QDateTime &dateTime, ParserAbstract::Mode mode, int trainrestrictions)
{
    if (currentRequestState != FahrplanNS::noneRequest) {
        return;
    }

    currentRequestState = FahrplanNS::searchJourneyRequest;
    hafasContext.seqNr = "";
    lastJourneyResultList = NULL;

    QString trainrestr = getTrainRestrictionsCodes(trainrestrictions);

    QByteArray postData = "";
    postData.append("<?xml version=\"1.0\" encoding=\"UTF-8\" ?><ReqC accessId=\"" + hafasHeader.accessid + "\" ver=\"" + hafasHeader.ver + "\" prod=\"" + hafasHeader.prod + "\" lang=\"EN\">");
    postData.append("<ConReq>");
    postData.append("<Start min=\"0\">");
    postData.append("<Station externalId=\"");
    postData.append(parseExternalIds(departureStation.id));
    postData.append("\" distance=\"0\"></Station>");
    postData.append("<Prod prod=\"");
    postData.append(trainrestr);
    postData.append("\"></Prod>");
    postData.append("</Start>");
    if (viaStation.id.isValid()) {
        postData.append("<Via min=\"0\">");
        postData.append("<Station externalId=\"");
        postData.append(parseExternalIds(viaStation.id));
        postData.append("\" distance=\"0\"></Station>");
        postData.append("<Prod prod=\"");
        postData.append(trainrestr);
        postData.append("\"></Prod>");
        postData.append("</Via>");
    }
    postData.append("<Dest min=\"0\">");
    postData.append("<Station externalId=\"");
    postData.append(parseExternalIds(arrivalStation.id));
    postData.append("\" distance=\"0\"></Station>");
    postData.append("</Dest>");
    postData.append("<ReqT time=\"");
    postData.append(dateTime.toString("hh:mm"));
    postData.append("\" date=\"");
    postData.append(dateTime.toString("yyyyMMdd"));
    postData.append("\" a=\"");
    postData.append((mode == Arrival) ? "1" : "0");
    postData.append("\"></ReqT>");

    if (mode == Arrival) {
        postData.append("<RFlags b=\"");
        postData.append("4"); //From count
        postData.append("\" f=\"");
        postData.append("1"); //To count
        postData.append("\"></RFlags>");
    } else {
        postData.append("<RFlags b=\"");
        postData.append("1"); //From count
        postData.append("\" f=\"");
        postData.append("4"); //To count
        postData.append("\"></RFlags>");
    }

    postData.append("<GISParameters><Front></Front><Back></Back></GISParameters>");
    postData.append("</ConReq>");
    postData.append("</ReqC>");

    sendHttpRequest(QUrl(baseXmlUrl), postData);
}

QByteArray ParserHafasXml::getStationsExternalIds(const QString &departureStation, const QString &arrivalStation, const QString &viaStation)
{
    QByteArray postData = "";
    postData.append("<?xml version=\"1.0\" encoding=\"UTF-8\" ?><ReqC accessId=\"" + hafasHeader.accessid + "\" ver=\"" + hafasHeader.ver + "\" prod=\"" + hafasHeader.prod + "\" lang=\"EN\">");

    if (!departureStation.isEmpty()) {
        postData.append("<LocValReq id=\"from\" maxNr=\"1\"><ReqLoc match= \"");
        postData.append(departureStation);
        postData.append("\" type=\"ST\"/></LocValReq>");
    }
    if (!arrivalStation.isEmpty()) {
        postData.append("<LocValReq id=\"to\" maxNr=\"1\"><ReqLoc match= \"");
        postData.append(arrivalStation);
        postData.append("\" type=\"ST\"/></LocValReq>");
    }
    if (!viaStation.isEmpty()) {
        postData.append("<LocValReq id=\"via\" maxNr=\"1\"><ReqLoc match= \"");
        postData.append(viaStation);
        postData.append("\" type=\"ST\"/></LocValReq>");
    }
    postData.append("</ReqC>");

    return postData;
}

QString ParserHafasXml::parseExternalIds(const QVariant &id) const
{
    QString extId;
    QRegExp l("L=(\\d+)");
    QRegExp u("U=(\\d+)");
    int i;

    i = l.indexIn(id.toString());
    if (i < 0 || l.capturedTexts().count() < 2)
        return QString();
    extId.append(l.capturedTexts().at(1));

    extId.append("#");

    i = u.indexIn(id.toString());
    if (i < 0 || u.capturedTexts().count() < 2)
        return QString();
    extId.append(u.capturedTexts().at(1));

    return extId;
}

void ParserHafasXml::parseSearchJourney(QNetworkReply *networkReply)
{
    lastJourneyResultList = new JourneyResultList();
    journeyDetailInlineData.clear();

    QBuffer readBuffer;
    readBuffer.setData(networkReply->readAll());
    readBuffer.open(QIODevice::ReadOnly);

    QXmlQuery query;
    query.bindVariable("path", &readBuffer);
    query.setQuery("doc($path)/ResC/Err//@text/string()");

    QStringList errorResult;
    if (!query.evaluateTo(&errorResult))
    {
        qDebug() << "parserHafasXml::ErrorTest - Query Failed";
    }

    if (errorResult.count() > 0 ) {
        emit errorOccured(errorResult.join("").trimmed());
        qWarning()<<"ParserHafasXml::parseSearchJourneyPart2:"<<errorResult.join("");
        return;
    }

    //Query for station infos
    query.setQuery("doc($path)/ResC/ConRes/ConnectionList/Connection/@id/string()");

    QStringList resultIds;
    if (!query.evaluateTo(&resultIds))
    {
        qDebug() << "parserHafasXml::getJourneyData 2 - Query Failed";
    }

    if (resultIds.count() <= 0) {
        emit journeyResult(lastJourneyResultList);
        return;
    }

    for(int i = 0;i<resultIds.count(); i++) {
        //qDebug()<<"Connection:"<<resultIds[i];

        query.setQuery("doc($path)/ResC/ConRes/ConnectionList/Connection[@id='" + resultIds[i] + "']/Overview/Date/string()");
        QStringList dateResult;
        if (!query.evaluateTo(&dateResult))
        {
            qDebug() << "parserHafasXml::getJourneyData 3 - Query Failed";
        }

        query.setQuery("doc($path)/ResC/ConRes/ConnectionList/Connection[@id='" + resultIds[i] + "']/Overview/Transfers/string()");
        QStringList transfersResult;
        if (!query.evaluateTo(&transfersResult))
        {
            qDebug() << "parserHafasXml::getJourneyData 4 - Query Failed";
        }

        query.setQuery("doc($path)/ResC/ConRes/ConnectionList/Connection[@id='" + resultIds[i] + "']/Overview/Duration/Time/string()");
        QStringList durationResult;
        if (!query.evaluateTo(&durationResult))
        {
            qDebug() << "parserHafasXml::getJourneyData 5 - Query Failed";
        }

        query.setQuery("doc($path)/ResC/ConRes/ConnectionList/Connection[@id='" + resultIds[i] + "']/Overview/Products/Product/@cat/string()");
        QStringList trainsResult;
        if (!query.evaluateTo(&trainsResult))
        {
            qDebug() << "parserHafasXml::getJourneyData 6 - Query Failed";
        }

        query.setQuery("doc($path)/ResC/ConRes/ConnectionList/Connection[@id='" + resultIds[i] + "']/Overview/Departure/BasicStop/Station/@name/string()");
        QStringList depResult;
        if (!query.evaluateTo(&depResult))
        {
            qDebug() << "parserHafasXml::getJourneyData 7 - Query Failed";
        }

        query.setQuery("doc($path)/ResC/ConRes/ConnectionList/Connection[@id='" + resultIds[i] + "']/Overview/Arrival/BasicStop/Station/@name/string()");
        QStringList arrResult;
        if (!query.evaluateTo(&arrResult))
        {
            qDebug() << "parserHafasXml::getJourneyData 8 - Query Failed";
        }

        query.setQuery("doc($path)/ResC/ConRes/ConnectionList/Connection[@id='" + resultIds[i] + "']/Overview/Departure/BasicStop/Dep/Time/string()");
        QStringList depTimeResult;
        if (!query.evaluateTo(&depTimeResult))
        {
            qDebug() << "parserHafasXml::getJourneyData 9 - Query Failed";
        }

        query.setQuery("doc($path)/ResC/ConRes/ConnectionList/Connection[@id='" + resultIds[i] + "']/Overview/Departure/BasicStop/Dep/Platform/Text/string()");
        QStringList depPlatResult;
        if (!query.evaluateTo(&depPlatResult))
        {
            qDebug() << "parserHafasXml::getJourneyData 10 - Query Failed";
        }

        query.setQuery("doc($path)/ResC/ConRes/ConnectionList/Connection[@id='" + resultIds[i] + "']/Overview/Arrival/BasicStop/Arr/Time/string()");
        QStringList arrTimeResult;
        if (!query.evaluateTo(&arrTimeResult))
        {
            qDebug() << "parserHafasXml::getJourneyData 11 - Query Failed";
        }

        query.setQuery("doc($path)/ResC/ConRes/ConnectionList/Connection[@id='" + resultIds[i] + "']/Overview/Arrival/BasicStop/Arr/Platform/Text/string()");
        QStringList arrPlatResult;
        if (!query.evaluateTo(&arrPlatResult))
        {
            qDebug() << "parserHafasXml::getJourneyData 12 - Query Failed";
        }

        query.setQuery("doc($path)/ResC/ConRes/ConnectionList/Connection[@id='" + resultIds[i] + "']/Overview/XMLHandle/@url/string()");
        QStringList xmlHandleResult;
        if (!query.evaluateTo(&xmlHandleResult))
        {
            qDebug() << "parserHafasXml::getJourneyData 13 - Query Failed";
        }

        QDate date = QDate::fromString(dateResult.join("").trimmed(), "yyyyMMdd");

        for (int j = 0; j < trainsResult.count(); j++) {
            trainsResult[j] = trainsResult[j].trimmed();
        }

        JourneyResultItem *item = new JourneyResultItem();
        item->setDate(date);
        item->setId(resultIds[i]);
        item->setTransfers(transfersResult.join("").trimmed());
        item->setDuration(cleanHafasDate(durationResult.join("").trimmed()));
        item->setMiscInfo("");
        item->setTrainType(trainsResult.join(", ").trimmed());
        item->setDepartureTime(cleanHafasDate(depTimeResult.join("").trimmed()));
        item->setArrivalTime(cleanHafasDate(arrTimeResult.join("").trimmed()));

        bool hasInline = false;
        QString internalData1 = xmlHandleResult.join("").trimmed();

        if (internalData1.count() > 0 && internalData1.indexOf("extxml.exe")) {
            hasInline = true;
        }

        if (internalData1.count() > 0) {
            internalData1.remove(0, internalData1.indexOf("query.exe") + 9);
            internalData1.prepend(baseUrl);
            item->setInternalData1(internalData1);
        } else {
            hasInline = true;
        }

        if (hasInline){
            journeyDetailRequestData.id = item->id();
            journeyDetailRequestData.date = item->date();
            journeyDetailRequestData.duration = item->duration();
            QByteArray data = readBuffer.buffer();
            JourneyDetailResultList *results = internalParseJourneyDetails(data);
            journeyDetailInlineData.append(results);
        }

        lastJourneyResultList->setDepartureStation(depResult.join("").trimmed());
        lastJourneyResultList->setArrivalStation(arrResult.join("").trimmed());
        lastJourneyResultList->setTimeInfo(date.toString());

        lastJourneyResultList->appendItem(item);
    }

    //Query for next and prev stuff
    query.setQuery("doc($path)/ResC/ConRes/ConResCtxt/string()");
    QStringList ConResCtxtResult;
    if (!query.evaluateTo(&ConResCtxtResult))
    {
        qDebug() << "parserHafasXml::getJourneyData 14 - Query Failed";
    }

    hafasContext.seqNr = ConResCtxtResult.join("");
    emit journeyResult(lastJourneyResultList);
}

void ParserHafasXml::searchJourneyLater()
{
    if (hafasContext.seqNr.isEmpty()) {
        emit errorOccured(tr("Internal error occured, going later is not possible"));
        return;
    }

    if (currentRequestState != FahrplanNS::noneRequest) {
        return;
    }

    currentRequestState = FahrplanNS::searchJourneyLaterRequest;

    QByteArray postData = "";
    postData.append("<?xml version=\"1.0\" encoding=\"UTF-8\" ?><ReqC accessId=\"" + hafasHeader.accessid + "\" ver=\"" + hafasHeader.ver + "\" prod=\"" + hafasHeader.prod + "\" lang=\"EN\">");
    postData.append("<ConScrReq scrDir=\"F\" nrCons=\"5\">");
    postData.append("<ConResCtxt>");
    postData.append(hafasContext.seqNr);
    postData.append("</ConResCtxt>");
    postData.append("</ConScrReq>");
    postData.append("</ReqC>");

    sendHttpRequest(QUrl(baseXmlUrl), postData);
}

void ParserHafasXml::searchJourneyEarlier()
{
    if (hafasContext.seqNr.isEmpty()) {
        emit errorOccured(tr("Internal error occured, going earlier is not possible"));
        return;
    }

    if (currentRequestState != FahrplanNS::noneRequest) {
        return;
    }

    currentRequestState = FahrplanNS::searchJourneyEarlierRequest;

    QByteArray postData = "";
    postData.append("<?xml version=\"1.0\" encoding=\"UTF-8\" ?><ReqC accessId=\"" + hafasHeader.accessid + "\" ver=\"" + hafasHeader.ver + "\" prod=\"" + hafasHeader.prod + "\" lang=\"EN\">");
    postData.append("<ConScrReq scrDir=\"B\" nrCons=\"5\">");
    postData.append("<ConResCtxt>");
    postData.append(hafasContext.seqNr);
    postData.append("</ConResCtxt>");
    postData.append("</ConScrReq>");
    postData.append("</ReqC>");

    sendHttpRequest(QUrl(baseXmlUrl), postData);
}

void ParserHafasXml::parseSearchLaterJourney(QNetworkReply *networkReply)
{
    parseSearchJourney(networkReply);
}

void ParserHafasXml::parseSearchEarlierJourney(QNetworkReply *networkReply)
{
    parseSearchJourney(networkReply);
}

void ParserHafasXml::getJourneyDetails(const QString &id)
{
    if (currentRequestState != FahrplanNS::noneRequest) {
        return;
    }

    journeyDetailRequestData.id = "";


    //Some hafasxml backend provide the detailsdata inline
    //if so our parser already stored them
    if (journeyDetailInlineData.count() > 0 ) {

        for (int i = 0; i < journeyDetailInlineData.count(); i++) {
            JourneyDetailResultList *item = journeyDetailInlineData.at(i);
            if (item->id() == id) {
                emit journeyDetailsResult(item);
                return;
            }
        }
        emit errorOccured(tr("Internal error occured: JourneyResultdata not present!"));
        return;
    }

    //It seams we don't have the detailsdata internaly stored,
    //So we fetch them remotly.
    if (lastJourneyResultList) {

        for (int i = 0; i < lastJourneyResultList->itemcount(); i++) {
            JourneyResultItem *item = lastJourneyResultList->getItem(i);
            if (item->id() == id) {
                currentRequestState = FahrplanNS::journeyDetailsRequest;
                journeyDetailRequestData.id = item->id();
                journeyDetailRequestData.date = item->date();
                journeyDetailRequestData.duration = item->duration();
                sendHttpRequest(item->internalData1(), "<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>");
                return;
            }
        }

    } else {
        emit errorOccured(tr("Internal error occured: JourneyResultdata not present!"));
    }
}

JourneyDetailResultList* ParserHafasXml::internalParseJourneyDetails(QByteArray data)
{
    qDebug() << ";;;;;;;;;;;;;;;" << data;
    JourneyDetailResultList *results = new JourneyDetailResultList();

    QBuffer readBuffer;
    readBuffer.setData(data);
    readBuffer.open(QIODevice::ReadOnly);

    QXmlQuery query;
    query.bindVariable("path", &readBuffer);
    query.setQuery("doc($path)/ResC/Err//@text/string()");

    QStringList errorResult;
    if (!query.evaluateTo(&errorResult))
    {
        qDebug() << "parserHafasXml::ErrorTest - Query Failed";
        return results;
    }

    if (errorResult.count() > 0 ) {
        emit errorOccured(errorResult.join("").trimmed());
        qWarning()<<"ParserHafasXml::internalParseJourneyDetails:"<<errorResult.join("");
        return results;
    }

    query.setQuery("doc($path)/ResC/ConRes//Connection[@id='" + journeyDetailRequestData.id + "']/ConSectionList/ConSection/Departure/BasicStop/Station/@name/string()");
    QStringList departureResults;
    if (!query.evaluateTo(&departureResults))
    {
        qDebug() << "parserHafasXml::parseJourneyDataDetails - Query 1 Failed";
    }
    query.setQuery("doc($path)/ResC/ConRes//Connection[@id='" + journeyDetailRequestData.id + "']/ConSectionList/ConSection/Arrival/BasicStop/Station/@name/string()");
    QStringList arrivalResults;
    if (!query.evaluateTo(&arrivalResults))
    {
        qDebug() << "parserHafasXml::parseJourneyDataDetails - Query 2 Failed";
    }

    query.setQuery("doc($path)/ResC/ConRes//Connection[@id='" + journeyDetailRequestData.id + "']/ConSectionList/ConSection/Departure/BasicStop/Location/Station/HafasName/Text/string()");
    QStringList departure2Results;
    if (!query.evaluateTo(&departure2Results))
    {
        qDebug() << "parserHafasXml::parseJourneyDataDetails - Query 1b Failed";
    }
    query.setQuery("doc($path)/ResC/ConRes//Connection[@id='" + journeyDetailRequestData.id + "']/ConSectionList/ConSection/Arrival/BasicStop/Location/Station/HafasName/Text/string()");
    QStringList arrival2Results;
    if (!query.evaluateTo(&arrival2Results))
    {
        qDebug() << "parserHafasXml::parseJourneyDataDetails - Query 2b Failed";
    }

    query.setQuery("doc($path)/ResC/ConRes//Connection[@id='" + journeyDetailRequestData.id + "']/ConSectionList/ConSection/Departure/BasicStop/Dep/Time/string()");
    QStringList depTimeResult;
    if (!query.evaluateTo(&depTimeResult))
    {
        qDebug() << "parserHafasXml::parseJourneyDataDetails - Query 3 Failed";
    }

    query.setQuery("doc($path)/ResC/ConRes//Connection[@id='" + journeyDetailRequestData.id + "']/ConSectionList/ConSection/Departure/BasicStop/Dep/Platform/Text/string()");
    QStringList depPlatResult;
    if (!query.evaluateTo(&depPlatResult))
    {
        qDebug() << "parserHafasXml::parseJourneyDataDetails - Query 4 Failed";
    }

    query.setQuery("doc($path)/ResC/ConRes//Connection[@id='" + journeyDetailRequestData.id + "']/ConSectionList/ConSection/Arrival/BasicStop/Arr/Time/string()");
    QStringList arrTimeResult;
    if (!query.evaluateTo(&arrTimeResult))
    {
        qDebug() << "parserHafasXml::parseJourneyDataDetails - Query 5 Failed";
    }

    query.setQuery("doc($path)/ResC/ConRes//Connection[@id='" + journeyDetailRequestData.id + "']/ConSectionList/ConSection/Arrival/BasicStop/Arr/Platform/Text/string()");
    QStringList arrPlatResult;
    if (!query.evaluateTo(&arrPlatResult))
    {
        qDebug() << "parserHafasXml::parseJourneyDataDetails - Query 6 Failed";
    }

    //It is possible, that the stationname is in two seperate fields
    if (departureResults.count() == 0 && departure2Results.count() > 0)
    {
        departureResults = departure2Results;
        arrivalResults = arrival2Results;
    }

    if (departureResults.count() == arrivalResults.count())
    {
        for(int i = 0; i < departureResults.count(); i++)
        {
            JourneyDetailResultItem *item = new JourneyDetailResultItem();

            /*
            qDebug()<<"   "<<"Journey "<<i;
            qDebug()<<"     DepartureSt:"<<departureResults[i].trimmed();
            qDebug()<<"     DepartureTime:"<<depTimeResult[i].trimmed();
            qDebug()<<"     DeparturePlatform:"<<depPlatResult[i].trimmed();
            qDebug()<<"     ArrivalSt:"<<arrivalResults[i].trimmed();
            qDebug()<<"     ArrivalTime:"<<arrTimeResult[i].trimmed();
            qDebug()<<"     ArrivalPlatform:"<<arrPlatResult[i].trimmed();
            */

            item->setDepartureDateTime(cleanHafasDateTime(depTimeResult[i].trimmed(), journeyDetailRequestData.date));
            item->setArrivalDateTime(cleanHafasDateTime(arrTimeResult[i].trimmed(), journeyDetailRequestData.date));
            item->setDepartureStation(departureResults[i].trimmed());
            item->setArrivalStation(arrivalResults[i].trimmed());
            if (depPlatResult[i].trimmed() != "")
            {
                item->setDepartureInfo(tr("Pl.") + " " + depPlatResult[i].trimmed());
            }
            if (arrPlatResult[i].trimmed() != "")
            {
                item->setArrivalInfo(tr("Pl.") + " " + arrPlatResult[i].trimmed());
            }

            //Check for train or if walking
            query.setQuery("doc($path)/ResC/ConRes//Connection[@id='" + journeyDetailRequestData.id + "']/ConSectionList/ConSection[" + QString::number(i + 1) + "]/Journey/JourneyAttributeList/JourneyAttribute/Attribute[@type='NAME']/AttributeVariant/Text/string()");
            QStringList trainResult;
            if (!query.evaluateTo(&trainResult))
            {
                qDebug() << "parserHafasXml::parseJourneyDataDetails - Query 7 Failed";
            }

            if (trainResult.count() > 0)
            {
                //qDebug()<<"     Train:"<<trainResult.join("").trimmed();
                item->setTrain(trainResult.join("").trimmed());
            } else
            {
                query.setQuery("doc($path)/ResC/ConRes//Connection[@id='" + journeyDetailRequestData.id + "']/ConSectionList/ConSection[" + QString::number(i + 1) + "]/Walk/Duration/Time/string()");
                QStringList walkResult;
                if (!query.evaluateTo(&walkResult))
                {
                   qDebug() << "parserHafasXml::parseJourneyDataDetails - Query 8 Failed";
                }

                //Maybe its a transfer?
                QStringList transferResult;
                if (walkResult.count() == 0) {
                    query.setQuery("doc($path)/ResC/ConRes//Connection[@id='" + journeyDetailRequestData.id + "']/ConSectionList/ConSection[" + QString::number(i + 1) + "]/Transfer/Duration/Time/string()");

                    if (!query.evaluateTo(&transferResult))
                    {
                       qDebug() << "parserHafasXml::parseJourneyDataDetails - Query 8b Failed";
                    }
                }

                //Maybe its gisroute?
                if (walkResult.count() == 0 && transferResult.count() == 0)
                {
                   query.setQuery("doc($path)/ResC/ConRes//Connection[@id='" + journeyDetailRequestData.id + "']/ConSectionList/ConSection[" + QString::number(i + 1) + "]/GisRoute/Duration/Time/string()");
                   QStringList gisrouteResult;
                   if (!query.evaluateTo(&gisrouteResult))
                   {
                       qDebug() << "parserHafasXml::parseJourneyDataDetails - Query 9 Failed";
                   }

                   //Ok its a gisroute
                   if (gisrouteResult.count() > 0)
                   {
                       query.setQuery("doc($path)/ResC/ConRes//Connection[@id='" + journeyDetailRequestData.id + "']/ConSectionList/ConSection[" + QString::number(i + 1) + "]/GisRoute/@type/string()");
                       QStringList gisroutetypeResult;
                       if (!query.evaluateTo(&gisroutetypeResult))
                       {
                           qDebug() << "parserHafasXml::parseJourneyDataDetails - Query 10 Failed";
                       }

                       QString gisrouteType = gisroutetypeResult.join("").trimmed();
                       int minutes = cleanHafasDate(gisrouteResult.join("").trimmed()).toInt();
                       if (gisrouteType == "FOOT")
                       {
                           item->setInfo(tr("Walk for %n min", "", minutes));
                       } else if (gisrouteType == "BIKE")
                       {
                           item->setInfo(tr("Use a Bike for %n min", "", minutes));
                       } else if (gisrouteType == "CAR")
                       {
                           item->setInfo(tr("Use a car for %n min", "", minutes));
                       } else if (gisrouteType == "TAXI")
                       {
                           item->setInfo(tr("Take a taxi for %n min", "", minutes));
                       }
                   }
                } else {

                    if (transferResult.count() > 0) {
                        item->setInfo(tr("Transfer for %n min", "", cleanHafasDate(transferResult.join("").trimmed()).toInt()));
                    }

                    if (walkResult.count() > 0) {
                        item->setInfo(tr("Walk for %n min", "", cleanHafasDate(walkResult.join("").trimmed()).toInt()));
                    }
                }
            }

            results->appendItem(item);
        }

        if (results->itemcount() > 0) {
            results->setDepartureStation(results->getItem(0)->departureStation());
            results->setArrivalStation(results->getItem(results->itemcount() - 1)->arrivalStation());

            for (int i=0; i < results->itemcount(); i++) {
                if (!results->getItem(i)->departureDateTime().isNull()) {
                    results->setDepartureDateTime(results->getItem(i)->departureDateTime());
                    break;
                }
            }

            for (int i=results->itemcount() -1; i >= 0; i--) {
                if (!results->getItem(i)->arrivalDateTime().isNull()) {
                    results->setArrivalDateTime(results->getItem(i)->arrivalDateTime());
                    break;
                }
            }

            results->setDuration(journeyDetailRequestData.duration);
            results->setId(journeyDetailRequestData.id);
        }

       return results;
    }

    emit errorOccured(tr("Internal error occured, Error parsing details data"));
    return results;
}

void ParserHafasXml::parseJourneyDetails(QNetworkReply *networkReply)
{
    JourneyDetailResultList *results = internalParseJourneyDetails(networkReply->readAll());
    emit journeyDetailsResult(results);
}

QDateTime ParserHafasXml::cleanHafasDateTime(const QString &time, QDate date)
{
    QDateTime result;
    result.setDate(date);
    QRegExp tmpRegexp = QRegExp("(\\d\\d)d(\\d\\d):(\\d\\d):(\\d\\d)");
    tmpRegexp.setMinimal(true);
    tmpRegexp.indexIn(time);

    result.setTime(QTime::fromString(tmpRegexp.cap(2) + tmpRegexp.cap(3) + tmpRegexp.cap(4), "hhmmss"));

    if (tmpRegexp.cap(1) != "00") {
        result = result.addDays(tmpRegexp.cap(1).toInt());
    }

    return result;
}

QString ParserHafasXml::cleanHafasDate(const QString &time)
{
    QRegExp tmpRegexp = QRegExp("(\\d\\d)d(\\d\\d):(\\d\\d):(\\d\\d)");
    tmpRegexp.setMinimal(true);
    tmpRegexp.indexIn(time);

    QString result = "";
    if (tmpRegexp.cap(1) != "00") {
        result.append(tmpRegexp.cap(1) + tr("d") + " ");
    }

    result.append(tmpRegexp.cap(2) + ":");
    result.append(tmpRegexp.cap(3));

    return result;
}

