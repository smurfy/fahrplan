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

#ifndef PARSER_NINETWO_H
#define PARSER_NINETWO_H

#include "parser_abstract.h"

#include <QMap>

class QNetworkReply;
/**
 * @brief The ParserNinetwo class
 * Parser for the 9292ov.nl dutch public transport route planner backend.
 * it uses the unofficial json backend
 */
class ParserNinetwo : public ParserAbstract
{
    Q_OBJECT
    struct {
        QDateTime firstOption;
        QDateTime lastOption;
        Station from;
        Station via;
        Station to;
        int restrictions;
        Mode mode;

    } lastsearch;

    struct {
        bool isValid;
        qreal latitude;
        qreal longitude;
    } lastCoordinates;

    typedef enum restrictions{
        all=0,
        trainsOnly=1,
        noFerry=2
    } restrictions;

    int timetableRestrictions;

public:
    ParserNinetwo(QObject* parent = 0);

    // ParserAbstract interface
public:
    static QString getName() { return QString("%1 / %2 (9292ov.nl)").arg(tr("Netherlands"), tr("Belgium")); }
    virtual QString name() { return getName(); }
    virtual QString shortName() { return "9292ov.nl"; }

public slots:
    void getTimeTableForStation(const Station &currentStation, const Station &directionStation, const QDateTime &dateTtime, ParserAbstract::Mode mode, int trainrestrictions);
    void findStationsByName(const QString &stationName);
    void findStationsByCoordinates(qreal longitude, qreal latitude);
    void searchJourney(const Station &departureStation,const Station &viaStation,const Station &arrivalStation,const QDateTime &dateTime,const ParserAbstract::Mode mode, int trainrestrictions);
    void searchJourneyLater();
    void searchJourneyEarlier();
    void getJourneyDetails(const QString &id);
    bool supportsGps() { return true; }
    bool supportsVia() { return true; }
    bool supportsTimeTable() { return true; }
    bool supportsTimeTableDirection() { return false; }
    QStringList getTrainRestrictions();

protected:
    void parseTimeTable(QNetworkReply *networkReply);
    void parseStationsByName(QNetworkReply *networkReply);
    void parseStationsByCoordinates(QNetworkReply *networkReply);
    void parseSearchJourney(QNetworkReply *networkReply);
    void parseSearchLaterJourney(QNetworkReply *networkReply);
    void parseSearchEarlierJourney(QNetworkReply *networkReply);
    void parseJourneyDetails(QNetworkReply *networkReply);
    QMap<QString, JourneyDetailResultList*> cachedResults;

private:
    void parseJourneyOption(const QVariantMap &object);
};

#endif // PARSER_NINETWO_H
