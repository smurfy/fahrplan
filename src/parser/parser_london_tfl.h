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

#ifndef PARSER_LONDONTFL_H
#define PARSER_LONDONTFL_H

#include "parser_abstract.h"

#include <QMap>

class QNetworkReply;
/**
 * @brief The ParserNinetwo class
 * Parser for the 9292ov.nl dutch public transport route planner backend.
 * it uses the unofficial json backend
 */
class ParserLondonTfl : public ParserAbstract
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
        all = 0,
        rail_tube_overground_dlr = 1,
        tube_overground_dlr = 2,
        bus_tram_tube_overground_dlr = 3,
        bus_tram = 4,
        bus = 5,
        rail = 6,
        tube = 7,
        overground = 8,
        dlr = 9
    } restrictions;

    int timetableRestrictions;

public:
    ParserLondonTfl(QObject* parent = 0);

    // ParserAbstract interface
public:
    static QString getName() { return QString("%1 (tfl.gov.uk)").arg(tr("London")); }
    virtual QString name() { return getName(); }
    virtual QString shortName() { return "Transport for London"; }

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
    void parseJourneyOption(const QVariantMap &object, const QString & id);

    QStringList getModesFromTrainRestrictions(int trainRestrictions);
    bool doesModeMatchTrainRestrictions(const QString & mode, int trainRestrictions);
    QStringList filterStopIdsByTrainRestrictions(const QStringList & stopIds, int trainRestrictions);
    void addTimeTableEntriesOfStopPoint(const QString & stopPointId, TimetableEntriesList & entriesList);

    QNetworkAccessManager *NetworkManagerTimeTableSubQuery;
};

#endif // PARSER_LONDONTFL_H
