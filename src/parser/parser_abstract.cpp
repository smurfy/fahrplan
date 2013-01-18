/*******************************************************************************

    This file is a part of Fahrplan for maemo 2009-2012

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

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTimer>

ParserAbstract::ParserAbstract(QObject *parent) :
    QObject(parent)
{
    NetworkManager = new QNetworkAccessManager(this);
    connect(NetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(networkReplyFinished(QNetworkReply*)));

    currentRequestState = FahrplanNS::noneRequest;

    requestTimeout = new QTimer();

    connect(requestTimeout, SIGNAL(timeout()), this, SLOT(networkReplyTimedOut()));
}

ParserAbstract::~ParserAbstract()
{
    delete requestTimeout;
    delete NetworkManager;
}

void ParserAbstract::networkReplyFinished(QNetworkReply *networkReply)
{
    FahrplanNS::curReqStates internalRequestState = currentRequestState;

    disconnect(lastRequest, SIGNAL(downloadProgress(qint64,qint64)), 0, 0);
    requestTimeout->stop();

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
        parseSearchEarlierJourney(networkReply);
    } else if (internalRequestState == FahrplanNS::journeyDetailsRequest) {
        parseJourneyDetails(networkReply);
    } else if (internalRequestState == FahrplanNS::getTimeTableForStationRequest) {
        parseTimeTable(networkReply);
    } else {
        qDebug()<<"Current request unhandled!";
    }
}

void ParserAbstract::cancelRequest()
{
    requestTimeout->stop();
    if (lastRequest) {
        lastRequest->abort();
    }
}

void ParserAbstract::sendHttpRequest(QUrl url, QByteArray data)
{
    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:13.0) Gecko/20100101 Firefox/13.0");
    request.setRawHeader("Cache-Control", "no-cache");

    if (data.isNull()) {
        lastRequest = NetworkManager->get(request);
    } else {
        lastRequest = NetworkManager->post(request, data);
    }

    requestTimeout->start(30000);

    connect(lastRequest, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(networkReplyDownloadProgress(qint64,qint64)));
}

void ParserAbstract::networkReplyDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    Q_UNUSED(bytesReceived)
    Q_UNUSED(bytesTotal)
    requestTimeout->stop();
    requestTimeout->start(30000);
}

void ParserAbstract::networkReplyTimedOut()
{
    cancelRequest();
    emit errorOccured(tr("Request timed out."));
}

void ParserAbstract::sendHttpRequest(QUrl url)
{
    sendHttpRequest(url, NULL);
}

bool ParserAbstract::supportsGps()
{
    return false;
}

bool ParserAbstract::supportsVia()
{
    return false;
}

bool ParserAbstract::supportsTimeTable()
{
    return false;
}

bool ParserAbstract::supportsTimeTableDirection()
{
    return false;
}

QStringList ParserAbstract::getTrainRestrictions()
{
    QStringList result;
    return result;
}

void ParserAbstract::getTimeTableForStation(const QString &stationName, const QString &directionStationName, const QDate &date, const QTime &time, Mode mode, int trainrestrictions)
{
    qDebug() << "ParserAbstract::getTimeTableForStation";
    Q_UNUSED(stationName);
    Q_UNUSED(directionStationName);
    Q_UNUSED(date);
    Q_UNUSED(time);
    Q_UNUSED(mode);
    Q_UNUSED(trainrestrictions);
}

void ParserAbstract::parseTimeTable(QNetworkReply *networkReply)
{
   Q_UNUSED(networkReply);
   qDebug() << "ParserAbstract::parseTimeTable";
}

void ParserAbstract::findStationsByName(const QString &stationName)
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

 void ParserAbstract::searchJourney(const QString &departureStation, const QString &arrivalStation, const QString &viaStation, const QDate &date, const QTime &time, Mode mode, int trainrestrictions)
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

 void ParserAbstract::parseSearchEarlierJourney(QNetworkReply *networkReply)
 {
     Q_UNUSED(networkReply);
     qDebug() << "ParserAbstract::parseSearchEarlierJourney";
 }

 void ParserAbstract::getJourneyDetails(const QString &id)
 {
     Q_UNUSED(id);
     qDebug() << "ParserAbstract::getJourneyDetails";
 }

 void ParserAbstract::parseJourneyDetails(QNetworkReply *networkReply)
 {
     Q_UNUSED(networkReply);
     qDebug() << "ParserAbstract::parseJourneyDetails";
 }

