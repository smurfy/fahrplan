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

#ifndef PARSER_131500COMAU_H
#define PARSER_131500COMAU_H

#include <QObject>
#include <QtXmlPatterns/QXmlQuery>
#include <QXmlResultItems>
#include "parser_abstract.h"

class Parser131500ComAu : public ParserAbstract
{
    Q_OBJECT
public:
    explicit Parser131500ComAu(QObject *parent = 0);
    static QString getName() { return "131500.com.au"; }
    QString name() { return "131500.com.au"; }
public slots:
    void findStationsByName(const QString &stationName);
    void findStationsByCoordinates(qreal longitude, qreal latitude);
    void searchJourney(const QString &departureStation, const QString &arrivalStation, const QString &viaStation, const QDate &date, const QTime &time, int mode, int trainrestrictions);
//    void searchJourneyLater();
//    void searchJourneyEarlier();
    void getJourneyDetails(const QString &id);
    bool supportsGps();
    bool supportsVia();
    QStringList getTrainRestrictions();

protected:
    void parseStationsByName(QNetworkReply *networkReply);
    void parseSearchJourney(QNetworkReply *networkReply);
//    void parseSearchLaterJourney(QNetworkReply *networkReply);
//    void parseSearchEalierJourney(QNetworkReply *networkReply);
private:
    JourneyResultList *lastJourneyResultList;
    JourneyDetailResultList *parseDetails(JourneyResultItem *journeyitem);
};

#endif // PARSER_131500COMAU_H
