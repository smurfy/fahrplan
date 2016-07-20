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
#include <QDomDocument>

#if defined(BUILD_FOR_QT5)
  #include <QUrlQuery>
#endif

// XML Schema Documentation:
// http://stefanwehrmeyer.com/projects/vbbxsd/

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
    TimetableEntriesList result;

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
            TimetableEntry item;

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
                } else if (txtDelay == "cancel") {
                    miscInfo = QString("<span style=\"color:#b30;\">%1</span>")
                               .arg(tr("Canceled!"));
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

            item.currentStation = station;
            item.destinationStation = dest;
            item.trainType = train;
            item.platform = xml.attributes().value("platform").toString().simplified();
            item.time = QTime::fromString(xml.attributes().value("fpTime").toString(), "hh:mm");

            QStringList announcements;
            while (!xml.atEnd()) {
                xml.readNext();

                if (xml.isStartElement() && xml.name() == "HIMMessage") {
                    QStringRef text = xml.attributes().value("header");
                    if (text.isEmpty())
                        text = xml.attributes().value("lead");
                    if (!text.isEmpty())
                        announcements << text.toString();
                }

                if (xml.isEndElement() && xml.name() == "Journey")
                   break;
            }

            QStringList info;
            if (announcements.count() > 0)
                info << QString("<span style=\"color:#b30;\">%1</span>")
                        .arg(announcements.join("<br />").replace("\n", "<br />"));

            if (!miscInfo.isEmpty())
                info << miscInfo;

            item.miscInfo = info.join("<br />");

            result << item;
        }

    }
    emit timetableResult(result);
}

void ParserHafasXml::parseTimeTableMode0(QNetworkReply *networkReply)
{
    TimetableEntriesList result;

    QString data = QString::fromUtf8(networkReply->readAll());

    QXmlStreamReader xml;
    xml.addData(data);

    while (!xml.atEnd()) {
        xml.readNext();
        if (xml.isStartElement() && (xml.name() == "STBJourney")) {
            TimetableEntry item;

            while (!xml.atEnd()) {
                xml.readNext();
                if (xml.isStartElement() && xml.name() == "Station") {
                    item.currentStation = xml.attributes().value("name").toString().simplified();
                    item.latitude = xml.attributes().value("y").toString().toInt();
                    item.longitude = xml.attributes().value("x").toString().toInt();
                }

                if (xml.isStartElement() && (xml.name() == "Dep" || xml.name() == "Arr" )) {
                    while (!xml.atEnd()) {
                        xml.readNext();
                        if (xml.isStartElement() && xml.name() == "Time") {
                            xml.readNext();
                            item.time = QTime::fromString(xml.text().toString(), "hh:mm");
                        }

                        if (xml.isStartElement() && xml.name() == "Platform") {
                            while (!xml.atEnd()) {
                                xml.readNext();
                                if (xml.isStartElement() && xml.name() == "Text") {
                                    xml.readNext();
                                    item.platform = xml.text().toString().simplified();
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
                                item.destinationStation = xml.text().toString().simplified();
                            }
                            if (currentAttributeType == "NAME") {
                                item.trainType = xml.text().toString().simplified();
                            }
                        }

                        if (xml.isEndElement() && xml.name() == "Attribute") {
                           break;
                        }
                    }
                }

                QStringList announcements;
                if (xml.isStartElement() && xml.name() == "IList") {
                    while (!xml.atEnd()) {
                        xml.readNext();

                        if (xml.isStartElement() && xml.name() == "I") {
                            const QStringRef text = xml.attributes().value("text");
                            if (!text.isEmpty())
                                announcements << text.toString();
                        }

                        if (xml.isEndElement() && xml.name() == "IList")
                           break;
                    }
                }

                QStringList info;
                if (announcements.count() > 0)
                    info << QString("<span style=\"color:#b30;\">%1</span>")
                            .arg(announcements.join("<br />").replace("\n", "<br />"));

                if (xml.isStartElement() && xml.name() == "JProg") {
                    xml.readNextStartElement();
                    if (xml.name() == "JStatus") {
                        xml.readNext();
                        const QString status = xml.text().toString();
                        if (status == "SCHEDULED")
                            info << tr("On-Time");
                        else if (status.endsWith("FAILURE"))
                            info << QString("<span style=\"color:#b30;\">%1</span>")
                                            .arg(tr("Canceled!"));
                    }
                }
                item.miscInfo = info.join("<br />");

                if (xml.isEndElement() && xml.name() == "STBJourney") {
                    result << item;
                    break;
                }
            }
        }
    }

    emit timetableResult(result);
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
    const StationsList result = internalParseStationsByName(data);
    emit stationsResult(result);
}

StationsList ParserHafasXml::internalParseStationsByName(const QString &data) const
{
    StationsList result;

    QXmlStreamReader xml;
    xml.addData(data);

    while (!xml.atEnd()) {
        xml.readNext();
        if (xml.isStartElement()) {
            if (xml.name() == "MLc")
            {
                Station item;
                item.id = xml.attributes().value("i").toString();
                item.name = xml.attributes().value("n").toString();
                item.type = xml.attributes().value("t").toString();
                item.latitude = xml.attributes().value("y").toString().toInt();
                item.longitude = xml.attributes().value("x").toString().toInt();
                QString miscInfo = xml.attributes().value("dist").toString();
                if (!miscInfo.isEmpty()) {
                    item.miscInfo = miscInfo + "m";
                }
                result << item;
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
    const StationsList result = internalParseStationsByName(data);
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

inline bool ParserHafasXml::parseXml(QDomDocument &doc, const QByteArray &xml)
{
    QString error;
    if (!doc.setContent(xml, &error)) {
        emit errorOccured(tr("Error parsing reponse from the server: %1").arg(error));
        return false;
    }

    const QDomNodeList errors = doc.elementsByTagName("Err");
    if (!errors.isEmpty()) {
        QStringList errorStrings;
        for (int i = 0; i < errors.count(); ++i)
            errorStrings << errors.at(i).toElement().attribute("text").trimmed();

        emit errorOccured(tr("%1 replied: \"%2\"").arg(name(), errorStrings.join(" ")));
        return false;
    }

    return true;
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

    QDomDocument doc;
    if (!parseXml(doc, networkReply->readAll()))
        return;

    const QDomNodeList connections = doc.elementsByTagName("Connection");
    for (int i = 0; i < connections.count(); ++i) {
        JourneyResultItem *item = new JourneyResultItem();
        item->setId(connections.at(i).toElement().attribute("id").trimmed());

        QDomElement overview = connections.at(i).firstChildElement("Overview");

        item->setDate(QDate::fromString(overview.firstChildElement("Date").text().trimmed(),
                                        "yyyyMMdd"));

        QDomElement depStop = overview.firstChildElement("Departure").firstChildElement("BasicStop");
        item->setDepartureTime(cleanHafasDate(depStop.firstChildElement("Dep")
                                                     .firstChildElement("Time")
                                                     .text()
                                                     .trimmed()));
        lastJourneyResultList->setDepartureStation(depStop.firstChildElement("Station")
                                                          .attribute("name")
                                                          .trimmed());

        QDomElement arrStation = overview.firstChildElement("Arrival").firstChildElement("BasicStop");
        item->setArrivalTime(cleanHafasDate(arrStation.firstChildElement("Arr")
                                                      .firstChildElement("Time")
                                                      .text()
                                                      .trimmed()));
        lastJourneyResultList->setArrivalStation(arrStation.firstChildElement("Station")
                                                           .attribute("name")
                                                           .trimmed());

        item->setTransfers(overview.firstChildElement("Transfers").text().trimmed());
        item->setDuration(cleanHafasDate(overview.firstChildElement("Duration")
                                                 .firstChildElement("Time")
                                                 .text()
                                                 .trimmed()));

        QStringList productNames;
        const QDomNodeList products = overview.firstChildElement("Products").childNodes();
        for (int j = 0; j < products.count(); ++j) {
            productNames << products.at(j).toElement().attribute("cat").trimmed();
        }
        item->setTrainType(productNames.join(tr(", ")));

        bool hasInline = !connections.at(i).firstChildElement("ConSectionList").isNull();
        if (!hasInline) {
            QString internalData1 = overview.firstChildElement("XMLHandle")
                                            .attribute("url")
                                            .trimmed();
            if (internalData1.contains("query.exe")) {
                internalData1.remove(0, internalData1.indexOf("query.exe") + 9);
                internalData1.prepend(baseUrl);
                item->setInternalData1(internalData1);
            }
        } else {
            journeyDetailRequestData.id = item->id();
            journeyDetailRequestData.date = item->date();
            journeyDetailRequestData.duration = item->duration();
            journeyDetailInlineData.append(internalParseJourneyDetails(connections.at(i)
                                                                                  .toElement()));
        }

        const QDomNodeList announces = connections.at(i).firstChildElement("IList").childNodes();
        if (announces.count() > 0) {
            QStringList announcements;
            for (int i = 0; i < announces.count(); ++i) {
                announcements << announces.at(i).toElement().attribute("text");
            }
            item->setMiscInfo(QString("<span style=\"color:#b30;\">%1</span>")
                              .arg(announcements.join("<br />").replace("\n", "<br />")));
        }

        lastJourneyResultList->setTimeInfo(item->date().toString());

        lastJourneyResultList->appendItem(item);
    }

    hafasContext.seqNr = doc.documentElement()
                            .firstChildElement("ConRes")
                            .firstChildElement("ConResCtxt")
                            .text();

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

JourneyDetailResultList* ParserHafasXml::internalParseJourneyDetails(const QDomElement &connection)
{
    JourneyDetailResultList *results = new JourneyDetailResultList();

    const QDomNodeList sections = connection.elementsByTagName("ConSection");
    for (int i = 0; i < sections.count(); ++i) {
        JourneyDetailResultItem *item = new JourneyDetailResultItem();

        const QDomNode section = sections.at(i);
        QDomElement stop;

        stop = section.firstChildElement("Departure").firstChildElement("BasicStop");
        const QString departure = stop.firstChildElement("Station")
                                      .attribute("name")
                                      .trimmed();
        const QString departure2 = stop.firstChildElement("Location")
                                       .firstChildElement("Station")
                                       .firstChildElement("HafasName")
                                       .firstChildElement("Text")
                                       .text()
                                       .trimmed();
        const QString depTime = stop.firstChildElement("Dep")
                                    .firstChildElement("Time")
                                    .text()
                                    .trimmed();
        const QString depPlatform = stop.firstChildElement("Dep")
                                        .firstChildElement("Platform")
                                        .firstChildElement("Text")
                                        .text()
                                        .trimmed();

        item->setDepartureStation(departure.isEmpty() ? departure2 : departure);
        item->setDepartureDateTime(cleanHafasDateTime(depTime, journeyDetailRequestData.date));
        if (!depPlatform.isEmpty())
            item->setDepartureInfo(tr("Pl. %1").arg(depPlatform));

        // Can be Journey, Walk, Transfer or GisRoute
        const QDomElement transport = section.firstChildElement("Departure").nextSiblingElement();
        if (transport.tagName() == "Journey") {
            QStringList trains;
            QStringList directions;
            QStringList infos;
            QStringList categories;
            QStringList numbers;

            const QDomNodeList attributes = transport.firstChildElement("JourneyAttributeList")
                                                     .childNodes();
            for (int j = 0; j < attributes.count(); ++j) {
                const QDomElement attr = attributes.at(j).toElement()
                                                         .firstChildElement("Attribute");
                if (attr.attribute("type") == "NAME") {
                    trains << attr.firstChildElement("AttributeVariant")
                                  .firstChildElement("Text")
                                  .text();
                } else if (attr.attribute("type") == "DIRECTION") {
                    directions << attr.firstChildElement("AttributeVariant")
                                      .firstChildElement("Text")
                                      .text();
                } else if (attr.attribute("type") == "CATEGORY") {
                    QDomElement el = attr.firstChildElement("AttributeVariant");
                    QString category;
                    do {
                        // Elements are ordered SHORT, NORMAL, LONG.
                        // We prefer NORMAL but it's not always available,
                        // so we store SHORT first and overwrite it with
                        // NORMAL if it's there.
                        if (el.attribute("type") == "SHORT") {
                            category = el.firstChildElement("Text").text().trimmed();
                        } else if (el.attribute("type") == "NORMAL") {
                            category = el.firstChildElement("Text").text().trimmed();
                            break; // We're not interested in LONG, so skip it
                        }
                        el = el.nextSiblingElement("AttributeVariant");
                    } while (!el.isNull());
                    categories << category;
                } else if (attr.attribute("type") == "NUMBER") {
                    numbers << attr.firstChildElement("AttributeVariant")
                                   .firstChildElement("Text")
                                   .text();
                } else if (!attr.hasAttribute("type")
                           && attr.hasAttribute("priority")
                           && attr.hasAttribute("code")) {
                    const QString text = attr.firstChildElement("AttributeVariant")
                                             .firstChildElement("Text")
                                             .text();
                    if (!text.isEmpty() && text != ".")
                        infos << text;
                }
            }

            if (trains.join("").isEmpty()) {
                // In case train info is not available try
                // to guess it from category + number
                item->setTrain(categories.join("") + " " + numbers.join(""));
            } else {
                item->setTrain(trains.join(" "));
            }
            item->setDirection(directions.join(" "));
            item->setInfo(infos.join(tr(", ")));
        } else if (transport.tagName() == "Walk" || transport.tagName() == "Transfer") {
            const QString duration = cleanHafasDate(transport.firstChildElement("Duration")
                                                             .firstChildElement("Time")
                                                             .text()
                                                             .trimmed());

            int distance;
            if (!transport.firstChildElement("Distance").isNull())
                distance = transport.firstChildElement("Distance").text().trimmed().toInt();
            else
                distance = transport.attribute("length").trimmed().toInt();

            QString type = transport.tagName() == "Walk" ? tr("Walk") : tr("Transfer");
            //: %1 can be "Walk" or "Transfer"
            item->setTrain(tr("%1 for %2 min").arg(type, duration));
            if (distance > 0)
                item->setInfo(tr("Distance %n meter(s)", "", distance));
        } else if (transport.tagName() == "GisRoute") {
            const QString duration = cleanHafasDate(transport.firstChildElement("Duration")
                                                         .firstChildElement("Time")
                                                         .text()
                                                         .trimmed());

            QString type;
            const QString t = transport.attribute("type").trimmed();
            if (t == "FOOT")
                type = tr("Walk");
            else if (t == "BIKE")
                type = tr("Use bike");
            else if (t == "TAXI")
                type = tr("Take taxi");
            else if (t == "CAR")
                type = tr("Drive car");
            if (!type.isEmpty()) {
                //: %1 can be "Walk", "Use bike", "Take taxi", or "Drive car"
                item->setTrain(tr("%1 for %2 min").arg(type, duration));
            }
        }

        stop = section.firstChildElement("Arrival").firstChildElement("BasicStop");
        const QString arrival = stop.firstChildElement("Station")
                                    .attribute("name")
                                    .trimmed();
        const QString arrival2 = stop.firstChildElement("Location")
                                     .firstChildElement("Station")
                                     .firstChildElement("HafasName")
                                     .firstChildElement("Text")
                                     .text()
                                     .trimmed();
        const QString arrTime = stop.firstChildElement("Arr")
                                    .firstChildElement("Time")
                                    .text()
                                    .trimmed();
        const QString arrPlatform = stop.firstChildElement("Arr")
                                        .firstChildElement("Platform")
                                        .firstChildElement("Text")
                                        .text()
                                        .trimmed();

        item->setArrivalStation(arrival.isEmpty() ? arrival2 : arrival);
        item->setArrivalDateTime(cleanHafasDateTime(arrTime, journeyDetailRequestData.date));
        if (!arrPlatform.isEmpty())
            item->setArrivalInfo(tr("Pl. %1").arg(arrPlatform));

        results->appendItem(item);
    }

    if (results->itemcount() > 0) {
        results->setDepartureStation(results->getItem(0)->departureStation());
        results->setArrivalStation(results->getItem(results->itemcount() - 1)->arrivalStation());

        for (int i = 0; i < results->itemcount(); ++i) {
            if (!results->getItem(i)->departureDateTime().isNull()) {
                results->setDepartureDateTime(results->getItem(i)->departureDateTime());
                break;
            }
        }

        for (int i = results->itemcount() - 1; i >= 0; --i) {
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

void ParserHafasXml::parseJourneyDetails(QNetworkReply *networkReply)
{
    QDomDocument doc;
    if (parseXml(doc, networkReply->readAll()))
        return;

    QDomElement connection;

    QDomNodeList connections = doc.elementsByTagName("Connection");
    for (int i = 0; i < connections.count(); ++i) {
        if (connections.at(i).toElement().attribute("id") == journeyDetailRequestData.id) {
            connection = connections.at(i).toElement();
            break;
        }
    }
    if (connection.isNull()) {
        qDebug() << "Connection with requested ID not found:" << journeyDetailRequestData.id;
        return;
    }

    JourneyDetailResultList *results = internalParseJourneyDetails(connection);

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
