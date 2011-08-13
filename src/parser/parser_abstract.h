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

#ifndef PARSER_ABSTRACT_H
#define PARSER_ABSTRACT_H

#include <QObject>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include "parser_definitions.h"

class ParserAbstract : public QObject
{
    Q_OBJECT
public:
    explicit ParserAbstract(QObject *parent = 0);

public slots:
    virtual void findStationsByName(QString stationName);
    virtual void findStationsByCoordinates(qreal longitude, qreal latitude);
    virtual void searchJourney(QString departureStation, QString arrivalStation, QString viaStation, QDate date, QTime time, int mode, int trainrestrictions);
    virtual bool supportsGps();

signals:
    void stationsResult(StationsResultList *result);
    void journeyResult(JourneyResultList *result);

protected slots:
    void networkReplyFinished(QNetworkReply*);

protected:
    QNetworkAccessManager *NetworkManager;
    FahrplanNS::curReqStates currentRequestState;

    virtual void parseStationsByName(QNetworkReply *networkReply);
    virtual void parseStationsByCoordinates(QNetworkReply *networkReply);
    virtual void parseSearchJourney(QNetworkReply *networkReply);
    void sendHttpRequest(QUrl url, QByteArray data);
    void sendHttpRequest(QUrl url);
};

#endif // PARSER_ABSTRACT_H
