/*******************************************************************************

    This file is a part of Fahrplan for maemo 2009-2010

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

     NetworkManager = new QNetworkAccessManager(this);
     connect(NetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(networkReplyFinished(QNetworkReply*)));

     //baseUrl = "http://fahrplan.oebb.at/bin/query.exe"; //OEB (fully operational/no RT) //no xmlhandle, detaildate already present!
     //baseUrl = "http://hafas.bene-system.com/bin/query.exe"; //hafas dev?? system? / no gps
     baseUrl = "http://reiseauskunft.bahn.de/bin/query.exe"; //bahn.de (journey stuff fails)
     //baseUrl = "http://fahrplan.sbb.ch/bin/query.exe"; //SBB (only returns one journey) / Xmlhandle present
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
    if (currentRequestState != Fahrplan::noneRequest) {
        return;
    }

    currentRequestState = Fahrplan::stationsByNameRequest;
    stationName.replace("\"", "");

    QByteArray postData = "";
    postData.append("<?xml version=\"1.0\" encoding=\"UTF-8\" ?><ReqC ver=\"1.1\" prod=\"String\" lang=\"EN\"><MLcReq><MLc n=\"");
    postData.append(stationName);
    postData.append("\" t=\"ST\" /></MLcReq></ReqC>");

    QNetworkRequest request;
    request.setUrl(QUrl(baseUrl));
    request.setRawHeader("User-Agent", "-");
    NetworkManager->post(request, postData);
}

void ParserHafasXml::findStationsByCoordinates(qreal longitude, qreal latitude)
{
    if (currentRequestState != Fahrplan::noneRequest) {
        return;
    }

    currentRequestState = Fahrplan::stationsByCoordinatesRequest;

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

    QNetworkRequest request;
    request.setUrl(QUrl(fullUrl));
    request.setRawHeader("User-Agent", "-");
    NetworkManager->get(request);
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

