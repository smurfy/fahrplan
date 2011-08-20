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

#ifndef PARSER_HAFASXML_H
#define PARSER_HAFASXML_H

#include <QObject>
#include <QtXmlPatterns/QXmlQuery>
#include <QXmlResultItems>
#include "parser_abstract.h"

struct ParserHafasXmlSearchJourneyRequestData
{
    int progress;
    QDate date;
    QTime time;
    int mode;
    int trainrestrictions;
};

struct ParserHafasXmlJourneyDetailRequestData
{
    QString id;
    QDate date;
    QString duration;
};

class ParserHafasXml : public ParserAbstract
{
    Q_OBJECT
public:
    explicit ParserHafasXml(QObject *parent = 0);
    static QString getName() { return "HafasXML"; }
    QString name() { return "HafasXML"; }

public slots:
    void findStationsByName(QString stationName);
    void findStationsByCoordinates(qreal longitude, qreal latitude);
    void searchJourney(QString departureStation, QString arrivalStation, QString viaStation, QDate date, QTime time, int mode, int trainrestrictions);
    void searchJourneyLater();
    void searchJourneyEarlier();
    void getJourneyDetails(QString id);
    bool supportsGps();
    bool supportsVia();
    QStringList getTrainRestrictions();

protected:
    QString baseUrl;
    void parseStationsByName(QNetworkReply *networkReply);
    void parseStationsByCoordinates(QNetworkReply *networkReply);
    void parseSearchJourney(QNetworkReply *networkReply);
    void parseSearchLaterJourney(QNetworkReply *networkReply);
    void parseSearchEalierJourney(QNetworkReply *networkReply);
    void parseJourneyDetails(QNetworkReply *networkReply);

private:
    QString conResCtxt;
    JourneyResultList *lastJourneyResultList;
    ParserHafasXmlSearchJourneyRequestData searchJourneyRequestData;
    ParserHafasXmlJourneyDetailRequestData journeyDetailRequestData;
    QList<JourneyDetailResultList*> journeyDetailInlineData;
    QString getTrainRestrictionsCodes(int trainrestrictions);
    QString cleanHafasDate(QString time);
    QDateTime cleanHafasDateTime(QString time, QDate date);
    void parseSearchJourneyPart1(QNetworkReply *networkReply);
    void parseSearchJourneyPart2(QNetworkReply *networkReply);
    JourneyDetailResultList* internalParseJourneyDetails(QByteArray data);
};

#endif // PARSER_HAFASXML_H
