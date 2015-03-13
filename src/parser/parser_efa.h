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

#ifndef PARSER_EFA_H
#define PARSER_EFA_H

#include <QDomDocument>
#include <QObject>
#include "parser_abstract.h"

class ParserEFA : public ParserAbstract
{
    Q_OBJECT
public:
    explicit ParserEFA(QObject *parent = 0);
    static QString getName() { return "EFA"; }
    virtual QString name() { return getName(); }
    virtual QString shortName() { return getName(); }

public slots:
    void findStationsByName(const QString &stationName);
    void findStationsByCoordinates(qreal longitude, qreal latitude);
    void searchJourney(const Station &departureStation, const Station &viaStation, const Station &arrivalStation, const QDateTime &dateTime, ParserAbstract::Mode mode, int trainrestrictions);
    void searchJourneyEarlier();
    void searchJourneyLater();
    void getJourneyDetails(const QString &id);
    void getTimeTableForStation(const Station &currentStation, const Station &, const QDateTime &dateTime, Mode mode, int);
    bool supportsGps();
    bool supportsVia();
    bool supportsTimeTable();
    bool supportsTimeTableDirection();
    void checkForError(QDomDocument *serverReplyDomDoc);
    QStringList getTrainRestrictions();

protected:
    QString baseRestUrl;
    void parseStationsByName(QNetworkReply *networkReply);
    void parseSearchJourney(QNetworkReply *networkReply);
    void parseStationsByCoordinates(QNetworkReply *networkReply);
    void parseTimeTable(QNetworkReply *networkReply);
    QDateTime parseItdDateTime(const QDomElement &element);
    QByteArray readNetworkReply(QNetworkReply *networkReply);

private:
    JourneyResultList *lastJourneyResultList;

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
