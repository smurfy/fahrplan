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

#include "parser_abstract.h"

ParserAbstract::ParserAbstract(QObject *parent)
{
    Q_UNUSED(parent);

    NetworkManager = new QNetworkAccessManager(this);
    connect(NetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(networkReplyFinished(QNetworkReply*)));

    currentRequestState = FahrplanNS::noneRequest;
}

void ParserAbstract::networkReplyFinished(QNetworkReply *networkReply)
{
    FahrplanNS::curReqStates internalRequestState = currentRequestState;
    lastRequest = NULL;

    //We overwrite the currentRequestState to noneRequest here, because this allows us to set a new one
    //if needed inside the parser
    currentRequestState = FahrplanNS::noneRequest;

    if (internalRequestState == FahrplanNS::stationsByNameRequest) {
        parseStationsByName(networkReply);
    } else if (internalRequestState == FahrplanNS::stationsByCoordinatesRequest) {
        parseStationsByCoordinates(networkReply);
    } else if (internalRequestState == FahrplanNS::searchJourneyRequest) {
        parseSearchJourney(networkReply);
    } else if (internalRequestState == FahrplanNS::searchJourneyLaterRequest) {
        parseSearchLaterJourney(networkReply);
    } else if (internalRequestState == FahrplanNS::searchJourneyEarlierRequest) {
        parseSearchEalierJourney(networkReply);
    } else {
        qDebug()<<"Current request unhandled!";
    }
}

void ParserAbstract::cancelRequest()
{
    if (lastRequest) {
        lastRequest->abort();
    }
}

void ParserAbstract::sendHttpRequest(QUrl url, QByteArray data)
{
    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("User-Agent", "-");

    if (data.isNull()) {
        lastRequest = NetworkManager->get(request);
    } else {
        lastRequest = NetworkManager->post(request, data);
    }
}

void ParserAbstract::sendHttpRequest(QUrl url)
{
    sendHttpRequest(url, NULL);
}

bool ParserAbstract::supportsGps()
{
    return false;
}

void ParserAbstract::findStationsByName(QString stationName)
{
    Q_UNUSED(stationName);
}

void ParserAbstract::findStationsByCoordinates(qreal longitude, qreal latitude)
{
    Q_UNUSED(longitude);
    Q_UNUSED(latitude);
}

 void ParserAbstract::parseStationsByName(QNetworkReply *networkReply)
 {
    Q_UNUSED(networkReply);
    qDebug() << "ParserAbstract::parseStationsByName";
 }

 void ParserAbstract::parseStationsByCoordinates(QNetworkReply *networkReply)
 {
     Q_UNUSED(networkReply);
     qDebug() << "ParserAbstract::parseStationsByCoordinates";
 }

 void ParserAbstract::searchJourney(QString departureStation, QString arrivalStation, QString viaStation, QDate date, QTime time, int mode, int trainrestrictions)
 {
     Q_UNUSED(departureStation);
     Q_UNUSED(arrivalStation);
     Q_UNUSED(viaStation);
     Q_UNUSED(date);
     Q_UNUSED(time);
     Q_UNUSED(mode);
     Q_UNUSED(trainrestrictions);
     qDebug() << "ParserAbstract::searchJourney";
 }

 void ParserAbstract::parseSearchJourney(QNetworkReply *networkReply)
 {
     Q_UNUSED(networkReply);
     qDebug() << "ParserAbstract::parseSearchJourney";
 }

 void ParserAbstract::searchJourneyLater()
 {
     qDebug() << "ParserAbstract::searchJourneyLater";
 }

 void ParserAbstract::searchJourneyEarlier()
 {
     qDebug() << "ParserAbstract::searchJourneyEarlier";
 }

 void ParserAbstract::parseSearchLaterJourney(QNetworkReply *networkReply)
 {
     Q_UNUSED(networkReply);
     qDebug() << "ParserAbstract::parseSearchLaterJourney";
 }

 void ParserAbstract::parseSearchEalierJourney(QNetworkReply *networkReply)
 {
     Q_UNUSED(networkReply);
     qDebug() << "ParserAbstract::parseSearchEalierJourney";
 }
