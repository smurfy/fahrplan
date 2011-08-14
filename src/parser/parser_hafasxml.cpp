/*******************************************************************************

    This file is a part of Fahrplan for maemo 2009-2011

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

#include "parser_hafasxml.h"

ParserHafasXml::ParserHafasXml(QObject *parent)
{
     Q_UNUSED(parent);

     //baseUrl = "http://fahrplan.oebb.at/bin/query.exe"; //OEB (fully operational/no RT) //no xmlhandle, detaildate already present!
     //baseUrl = "http://hafas.bene-system.com/bin/query.exe"; //hafas dev?? system? / no gps
     //baseUrl = "http://reiseauskunft.bahn.de/bin/query.exe"; //bahn.de (journey stuff fails)
     baseUrl = "http://fahrplan.sbb.ch/bin/query.exe"; //SBB (only returns one journey) / Xmlhandle present
     //baseUrl = "http://www.fahrplaner.de/hafas/query.exe"; //?? No Gps, returns only one result
     //baseUrl = "http://www.rejseplanen.dk/bin/query.exe";//?? No Gps, returns only one result //no xmlhandle, detaildate already present!
     //baseUrl = "http://airs1.septa.org/bin/query.exe";// not working at all

}

bool ParserHafasXml::supportsGps()
{
    return true;
}

void ParserHafasXml::findStationsByName(QString stationName)
{
    if (currentRequestState != FahrplanNS::noneRequest) {
        return;
    }

    currentRequestState = FahrplanNS::stationsByNameRequest;
    stationName.replace("\"", "");

    QByteArray postData = "";
    postData.append("<?xml version=\"1.0\" encoding=\"UTF-8\" ?><ReqC ver=\"1.1\" prod=\"String\" lang=\"EN\"><MLcReq><MLc n=\"");
    postData.append(stationName);
    postData.append("\" t=\"ST\" /></MLcReq></ReqC>");

    sendHttpRequest(QUrl(baseUrl), postData);
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

    QString fullUrl = baseUrl + "/eol?look_x=" +
                      sLongitude +
                      "&look_y=" +
                      sLatitude +
                      "&performLocating=2&tpl=stopsnear&L=vs_java&look_maxdist=5000&look_maxno=40";

    sendHttpRequest(QUrl(fullUrl));
}

void ParserHafasXml::parseStationsByName(QNetworkReply *networkReply)
{
    StationsResultList result;

    QXmlStreamReader xml;
    xml.addData(networkReply->readAll());

    while (!xml.atEnd()) {
        xml.readNext();
        if (xml.isStartElement()) {
            if (xml.name() == "MLc")
            {
                StationsResultItem *item = new StationsResultItem();
                item->setStationName(xml.attributes().value("n").toString());
                item->setStationType(xml.attributes().value("t").toString());
                item->setLongitude(xml.attributes().value("x").toString().toInt());
                item->setLatitude(xml.attributes().value("y").toString().toInt());
                QString miscInfo = xml.attributes().value("dist").toString();
                if (!miscInfo.isEmpty()) {
                    item->setMiscInfo(miscInfo + "m");
                }
                result.appendItem(item);
            }
        }
    }
    if (xml.error() && xml.error() != QXmlStreamReader::PrematureEndOfDocumentError) {
        qWarning() << "XML ERROR:" << xml.lineNumber() << ": " << xml.errorString();
    }

    emit stationsResult(&result);
}


void ParserHafasXml::parseStationsByCoordinates(QNetworkReply *networkReply)
{
    //Both responses are similar so we use the same parser
    parseStationsByName(networkReply);
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

void ParserHafasXml::searchJourney(QString departureStation, QString arrivalStation, QString viaStation, QDate date, QTime time, int mode, int trainrestrictions)
{
    Q_UNUSED(viaStation);
    if (currentRequestState != FahrplanNS::noneRequest) {
        return;
    }

    currentRequestState = FahrplanNS::searchJourneyRequest;
    searchJourneyRequestData.progress = 1;
    searchJourneyRequestData.date = date;
    searchJourneyRequestData.time = time;
    searchJourneyRequestData.mode = mode;
    searchJourneyRequestData.trainrestrictions = trainrestrictions;

    //First Request, to get external ids
    QByteArray postData = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?><ReqC ver=\"1.1\" prod=\"String\" lang=\"EN\">";
    postData.append("<LocValReq id=\"from\" maxNr=\"1\"><ReqLoc match= \"");
    postData.append(departureStation);
    postData.append("\" type=\"ST\"/></LocValReq>");
    postData.append("<LocValReq id=\"to\" maxNr=\"1\"><ReqLoc match= \"");
    postData.append(arrivalStation);
    postData.append("\" type=\"ST\"/></LocValReq>");
    postData.append("</ReqC>");

    sendHttpRequest(QUrl(baseUrl), postData);
}

void ParserHafasXml::parseSearchJourney(QNetworkReply *networkReply)
{
    switch (searchJourneyRequestData.progress) {
    case 1:
        parseSearchJourneyPart1(networkReply);
        break;
    case 2:
        parseSearchJourneyPart2(networkReply);
        break;
    }
}

void ParserHafasXml::parseSearchJourneyPart1(QNetworkReply *networkReply)
{
    QString departureStationId = "";
    QString viaStationId = "";
    QString arrivalStationId = "";

    QXmlStreamReader xml;
    xml.addData(networkReply->readAll());

    //Parsing the Response for External Id
    while (!xml.atEnd()) {
        xml.readNext();
        if (xml.isStartElement() && xml.name() == "LocValRes") {
            QString type = xml.attributes().value("id").toString();
            while (!xml.atEnd()) {
                xml.readNext();
                if (xml.isStartElement() && xml.name() == "Station") {
                    if (type == "from") {
                        departureStationId = xml.attributes().value("externalId").toString();
                    }
                    if (type == "via") {
                        viaStationId = xml.attributes().value("externalId").toString();
                    }
                    if (type == "to") {
                        arrivalStationId = xml.attributes().value("externalId").toString();
                    }
                }
                if (xml.isEndElement() && xml.name() == "LocValRes") {
                   break;
               }
            }
        }
    }
    if (xml.error() && xml.error() != QXmlStreamReader::PrematureEndOfDocumentError) {
        qWarning() << "XML ERROR:" << xml.lineNumber() << ": " << xml.errorString();
    }

    if (!departureStationId.isEmpty() && !arrivalStationId.isEmpty()) {

        currentRequestState = FahrplanNS::searchJourneyRequest;
        searchJourneyRequestData.progress = 2;

        QString trainrestr = getTrainRestrictionsCodes(searchJourneyRequestData.trainrestrictions);

        //Send the search request Itself
        QByteArray postData = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?><ReqC ver=\"1.1\" prod=\"String\" lang=\"EN\">";
        postData.append("<ConReq>");
        postData.append("<Start min=\"0\">");
        postData.append("<Station externalId=\"");
        postData.append(departureStationId);
        postData.append("\" distance=\"0\"></Station>");
        postData.append("<Prod prod=\"");
        postData.append(trainrestr);
        postData.append("\"></Prod>");
        postData.append("</Start>");
        postData.append("<Dest min=\"0\">");
        postData.append("<Station externalId=\"");
        postData.append(arrivalStationId);
        postData.append("\" distance=\"0\"></Station>");
        postData.append("</Dest>");
        postData.append("<Via></Via>");
        postData.append("<ReqT time=\"");
        postData.append(searchJourneyRequestData.time.toString("hh:mm"));
        postData.append("\" date=\"");
        postData.append(searchJourneyRequestData.date.toString("yyyyMMdd"));
        postData.append("\" a=\"");
        postData.append((searchJourneyRequestData.mode == 0) ? "1" : "0");
        postData.append("\"></ReqT>");

        if (searchJourneyRequestData.mode == 0) {
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

        sendHttpRequest(QUrl(baseUrl), postData);
    } else {
        //TODO: emit some kind of error signal the gui can hook
        qWarning()<<"ParserHafasXml::parseSearchJourneyPart1: MISSING External Ids!";
    }
}

void ParserHafasXml::parseSearchJourneyPart2(QNetworkReply *networkReply)
{
    JourneyResultList results;

    QBuffer readBuffer;
    readBuffer.setData(networkReply->readAll());
    readBuffer.open(QIODevice::ReadOnly);

    QXmlQuery query;
    query.bindVariable("path", &readBuffer);
    query.setQuery("doc($path)/ResC/Err//@text/string()");

    QStringList errorResult;
    if (!query.evaluateTo(&errorResult))
    {
        //TODO: emit some kind of error signal the gui can hook
        qDebug() << "parserHafasXml::ErrorTest - Query Failed";
    }

    if (errorResult.count() > 0 ) {
        //TODO: emit some kind of error signal the gui can hook
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
        emit journeyResult(&results);
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

        QString internalData1 = xmlHandleResult.join("").trimmed();
        if (internalData1.count() > 0) {
            internalData1.remove(0, internalData1.indexOf("query.exe") + 9);
            internalData1.prepend(baseUrl);
            item->setInternalData1(internalData1);
        } else {
            qWarning()<<"Todo for inline Details Data!";
            //item.detailsInfo = parseJourneyDataDetails(data, resultIds[i], date);
        }

        results.setDepartureStation(depResult.join("").trimmed());
        results.setArrivalStation(arrResult.join("").trimmed());
        results.setTimeInfo(date.toString());

        results.appendItem(item);
    }

    //Query for next and prev stuff
    query.setQuery("doc($path)/ResC/ConRes/ConResCtxt/string()");
    QStringList ConResCtxtResult;
    if (!query.evaluateTo(&ConResCtxtResult))
    {
        qDebug() << "parserHafasXml::getJourneyData 14 - Query Failed";
    }

    qDebug()<<"EARLIER/LATER"<<ConResCtxtResult.join("");

    /*
    results.laterUrl = "F" + ConResCtxtResult.join("");
    results.earlierUrl = "B" + ConResCtxtResult.join("");
    results.items = lastItems;
    */

    emit journeyResult(&results);
}

QDateTime ParserHafasXml::cleanHafasDateTime(QString time, QDate date)
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

QString ParserHafasXml::cleanHafasDate(QString time)
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

