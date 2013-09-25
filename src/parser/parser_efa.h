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

#ifndef PARSER_EFA_H
#define PARSER_EFA_H

#include <QObject>
#include <QXmlResultItems>
#include "parser_abstract.h"

class ParserEFA : public ParserAbstract
{
    Q_OBJECT
public:
    explicit ParserEFA(QObject *parent = 0);
    static QString getName() { return "ptv.vic.gov.au"; }
    QString name() { return "ptv.vic.gov.au"; }
public slots:
    virtual void findStationsByName(const QString &stationName);
    virtual void findStationsByCoordinates(double longitude, double latitude);
    virtual void searchJourney(const Station &departureStation, const Station &viaStation, const Station &arrivalStation, const QDateTime &dateTime, ParserAbstract::Mode mode, int trainrestrictions);
    virtual void searchJourneyEarlier();
    virtual void searchJourneyLater();
    virtual void getJourneyDetails(const QString &id);
    virtual void getTimeTableForStation(const Station &currentStation, const Station &, const QDateTime &dateTime, Mode mode, int);
    virtual bool supportsGps();
    virtual bool supportsVia();
    virtual bool supportsTimeTable();
    virtual bool supportsTimeTableDirection();
    QStringList getTrainRestrictions();

protected:
    QString baseRestUrl;
    virtual void parseStationsByName(QNetworkReply *networkReply);
    virtual void parseSearchJourney(QNetworkReply *networkReply);
    virtual void parseStationsByCoordinates(QNetworkReply *networkReply);
    virtual void parseTimeTable(QNetworkReply *networkReply);


private:
    JourneyResultList *lastJourneyResultList;
    JourneyDetailResultList *parseDetails(JourneyResultItem *journeyitem);

    struct {
        bool isValid;
        Station departureStation;
        Station arrivalStation;
        Station viaStation;
        QDateTime dateTime;
        Mode mode;
        int trainrestrictions;
    } m_searchJourneyParameters;

    struct {
        bool isValid;
        Station currentStation;
        Station directionStation;
        QDateTime dateTime;
        Mode mode;
        int trainrestrictions;
    } m_timeTableForStationParameters;

    QDateTime m_earliestArrival, m_latestResultDeparture;



};

#endif // PARSER_EFA_H
