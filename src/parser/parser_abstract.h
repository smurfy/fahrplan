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

#ifndef PARSER_ABSTRACT_H
#define PARSER_ABSTRACT_H

#include <QObject>
#include <QStringList>
#include "parser_definitions.h"

class QNetworkAccessManager;
class QNetworkReply;
class QTimer;
class QUrl;

class ParserAbstract : public QObject
{
    Q_OBJECT
    Q_ENUMS(Mode)

public:
    enum Mode { Arrival = 0, Departure = 1 };

    explicit ParserAbstract(QObject *parent = 0);
    ~ParserAbstract();

    static QString getName() { return "Abstract"; }
    virtual QString name() { return "Abstract"; }

public slots:
    virtual void getTimeTableForStation(const QString &stationName, const QString &directionStationName, const QDate &date, const QTime &time, Mode mode, int trainrestrictions);
    virtual void findStationsByName(const QString &stationName);
    virtual void findStationsByCoordinates(qreal longitude, qreal latitude);
    virtual void searchJourney(const QString &departureStation, const QString &arrivalStation, const QString &viaStation, const QDate &date, const QTime &time, Mode mode, int trainrestrictions);
    virtual void searchJourneyLater();
    virtual void searchJourneyEarlier();
    virtual void getJourneyDetails(const QString &id);
    virtual bool supportsGps();
    virtual bool supportsVia();
    virtual bool supportsTimeTable();
    virtual bool supportsTimeTableDirection();
    virtual QStringList getTrainRestrictions();
    void cancelRequest();

signals:
    void stationsResult(StationsResultList *result);
    void journeyResult(JourneyResultList *result);
    void journeyDetailsResult(JourneyDetailResultList *result);
    void timeTableResult(TimeTableResultList *result);
    void errorOccured(QString msg);

protected slots:
    void networkReplyFinished(QNetworkReply*);
    void networkReplyDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void networkReplyTimedOut();

protected:
    QNetworkAccessManager *NetworkManager;
    FahrplanNS::curReqStates currentRequestState;
    QNetworkReply *lastRequest;
    QTimer *requestTimeout;

    virtual void parseTimeTable(QNetworkReply *networkReply);
    virtual void parseStationsByName(QNetworkReply *networkReply);
    virtual void parseStationsByCoordinates(QNetworkReply *networkReply);
    virtual void parseSearchJourney(QNetworkReply *networkReply);
    virtual void parseSearchLaterJourney(QNetworkReply *networkReply);
    virtual void parseSearchEarlierJourney(QNetworkReply *networkReply);
    virtual void parseJourneyDetails(QNetworkReply *networkReply);
    void sendHttpRequest(QUrl url, QByteArray data);
    void sendHttpRequest(QUrl url);
};

#endif // PARSER_ABSTRACT_H
