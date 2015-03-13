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

#ifndef PARSER_HAFASXML_H
#define PARSER_HAFASXML_H

#include <QObject>
#include <QDomElement>
#include "parser_abstract.h"

struct ParserHafasXmlJourneyDetailRequestData
{
    QString id;
    QDate date;
    QString duration;
};

struct ParserHafasXmlHeader
{
    QString prod;
    QString ver;
    QString accessid;
};

struct ParserHafasXmlExternalIds
{
    QString departureId;
    QString viaId;
    QString arrivalId;
};

struct ParserHafasXmlContext
{
    QString seqNr;
    QString ident;
    QString ld;
};

class ParserHafasXml : public ParserAbstract
{
    Q_OBJECT
public:
    explicit ParserHafasXml(QObject *parent = 0);
    static QString getName() { return "HafasXML"; }
    virtual QString name() { return getName(); }
    virtual QString shortName() { return getName(); }

public slots:
    void getTimeTableForStation(const Station &currentStation, const Station &directionStation, const QDateTime &dateTime, ParserAbstract::Mode mode, int trainrestrictions);
    void findStationsByName(const QString &stationName);
    void findStationsByCoordinates(qreal longitude, qreal latitude);
    void searchJourney(const Station &departureStation, const Station &viaStation, const Station &arrivalStation, const QDateTime &dateTime, ParserAbstract::Mode mode, int trainrestrictions);
    void searchJourneyLater();
    void searchJourneyEarlier();
    void getJourneyDetails(const QString &id);
    bool supportsGps();
    bool supportsVia();
    bool supportsTimeTable();
    bool supportsTimeTableDirection();
    QStringList getTrainRestrictions();

protected:
    QString baseXmlUrl;
    QString baseUrl;
    QString baseSTTableUrl;
    ParserHafasXmlHeader hafasHeader;
    ParserHafasXmlJourneyDetailRequestData journeyDetailRequestData;
    ParserHafasXmlContext hafasContext;
    int STTableMode;
    void parseTimeTable(QNetworkReply *networkReply);
    void parseStationsByName(QNetworkReply *networkReply);
    void parseStationsByCoordinates(QNetworkReply *networkReply);
    void parseSearchJourney(QNetworkReply *networkReply);
    void parseSearchLaterJourney(QNetworkReply *networkReply);
    void parseSearchEarlierJourney(QNetworkReply *networkReply);
    void parseJourneyDetails(QNetworkReply *networkReply);
    virtual QString getTrainRestrictionsCodes(int trainrestrictions);

    JourneyResultList *lastJourneyResultList;
    QList<JourneyDetailResultList*> journeyDetailInlineData;
    StationsList internalParseStationsByName(const QString &data) const;

private:
    inline bool parseXml(QDomDocument &doc, const QByteArray &xml);
    QString parseExternalIds(const QVariant &id) const;

    QString cleanHafasDate(const QString &time);
    QDateTime cleanHafasDateTime(const QString &time, QDate date);
    QByteArray getStationsExternalIds(const QString &departureStation, const QString &arrivalStation, const QString &viaStation);
    void parseTimeTableMode1(QNetworkReply *networkReply);
    void parseTimeTableMode0(QNetworkReply *networkReply);
    JourneyDetailResultList* internalParseJourneyDetails(const QDomElement &connection);
};

#endif // PARSER_HAFASXML_H
