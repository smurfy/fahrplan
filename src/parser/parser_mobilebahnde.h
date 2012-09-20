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

#ifndef PARSER_MOBILEBAHNDE_H
#define PARSER_MOBILEBAHNDE_H

#include <QObject>
#include <QtNetwork>
#include <QtXmlPatterns/QXmlQuery>
#include <QXmlResultItems>
#include "parser_hafasxml.h"

struct ParserMobileBahnDeSearchJourneyRequestData
{
    int progress;
    QDate date;
    QTime time;
    ParserAbstract::Mode mode;
    int trainrestrictions;
    QString departureStation;
    QString viaStation;
    QString arrivalStation;
};

/*
 * bahn.de parser based for findstationby name and coordinates on the
 * hafasxml interface. for everything else we parse the bahn.de mobile website.
 */
class ParserMobileBahnDe : public ParserHafasXml
{
    Q_OBJECT
public:
    explicit ParserMobileBahnDe(QObject *parent = 0);
    static QString getName() { return "bahn.de"; }
    QString name() { return "bahn.de"; }

public slots:
    void searchJourney(const QString &departureStation, const QString &arrivalStation, const QString &viaStation, const QDate &date, const QTime &time, Mode mode, int trainrestrictions);
    void searchJourneyLater();
    void searchJourneyEarlier();
    void getJourneyDetails(const QString &id);
    bool supportsGps();
    bool supportsVia();
    bool supportsTimeTable();
    QStringList getTrainRestrictions();

protected:
    void parseSearchJourney(QNetworkReply *networkReply);
    void parseSearchLaterJourney(QNetworkReply *networkReply);
    void parseSearchEalierJourney(QNetworkReply *networkReply);
    void parseJourneyDetails(QNetworkReply *networkReply);

private:
    QString lastLaterUrl;
    QString lastEarlierUrl;
    JourneyResultList *lastJourneyResultList;
    ParserMobileBahnDeSearchJourneyRequestData searchJourneyRequestData;
    ParserHafasXmlJourneyDetailRequestData journeyDetailRequestData;
    void parseSearchJourneyCheckForId(QNetworkReply *networkReply);
    QString getTrainRestrictionsCodes(int trainrestrictions);
    void parseSearchJourneyPart1(const QString &data);
    void parseSearchJourneyPart2(const QString &data);
};

#endif // PARSER_MOBILEBAHNDE_H
