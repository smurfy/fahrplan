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

#ifndef PARSER_RESROBOT_H
#define PARSER_RESROBOT_H

#include "parser_abstract.h"

#include <QVariantMap>
#include <QHash>

/*
 * Some info on this API:
 *   * ResRobot enables searching for public transport connections in all of Sweden.
 *   * This backend actually uses two API's: "ResRobot Sök Resa" and "ResRobot Stolptidtabeller".
 *   * The super versions of the API should contain realtime info in the responses, but is expected
 *     to be slower.
 *   * Results are in XML or JSON.
 *   * ResRobot web frontend: http://resrobot.se/
 *   * More API info: https://www.trafiklab.se/api/resrobot-sok-resa
 *   * Backend project: https://www.trafiklab.se/projekt/fahrplan
 *   * Report API issues at https://kundo.se/org/trafiklabse/
 *
 * Issues and limitations:
 *   * JSON results can be strange sometimes. If a variable that normally contains a list only has
 *     one item, the variable itself holds the value of that item. The workaround in this class is
 *     to call a function with potential list variables and wrap the content in a list if it isn't
 *     a list already.
 *   * Via stations in search queries are not supported.
 *   * Timetable searching is limited to departures, and it is not possible to specify a time
 *     (the search is always "now").
 *   * Searching for station names returns a lot of noise - places with names very
 *     unlikely to be what the user wants. This is mitigated with extra filtering.
 *   * Documentation of realtime info is bad, and realtime data have so far only been spotted in
 *     timetable results.
 *   * There is no public documentation for the remark codes.
 *   * The transport mode codes aren't reliable. The API can return stuff that isn't documented.
 *   * Sometimes realtime info is missing due to timeout when the server is fetching the info from
 *     the external source.
 */

class ParserResRobot : public ParserAbstract
{
    Q_OBJECT
public:
    explicit ParserResRobot(QObject *parent = 0);
    virtual ~ParserResRobot();

    static QString getName() { return QString("%1 (resrobot.se)").arg(tr("Sweden")); }
    virtual QString name() { return getName(); }
    virtual QString shortName() { return "resrobot.se"; }

public slots:
    virtual bool supportsGps();
    virtual bool supportsVia();
    virtual bool supportsTimeTable();
    virtual bool supportsTimeTableDirection();
    virtual QStringList getTrainRestrictions();
    virtual void findStationsByName(const QString &stationName);
    virtual void findStationsByCoordinates(qreal longitude, qreal latitude);
    virtual void getTimeTableForStation(const Station &currentStation,
                                        const Station &directionStation,
                                        const QDateTime &datetime,
                                        ParserAbstract::Mode mode,
                                        int trainrestrictions);
    virtual void searchJourney(const Station &departureStation, const Station &viaStation,
                               const Station &arrivalStation, const QDateTime &dateTime,
                               ParserAbstract::Mode mode, int trainrestrictions);
    virtual void searchJourneyLater();
    virtual void searchJourneyEarlier();
    virtual void getJourneyDetails(const QString &id);
    virtual void clearJourney();

protected:
    virtual void parseTimeTable(QNetworkReply *networkReply);
    virtual void parseStationsByName(QNetworkReply *networkReply);
    virtual void parseStationsByCoordinates(QNetworkReply *networkReply);
    virtual void parseSearchJourney(QNetworkReply *networkReply);
    virtual void parseSearchLaterJourney(QNetworkReply *networkReply);
    virtual void parseSearchEarlierJourney(QNetworkReply *networkReply);
    virtual void parseJourneyDetails(QNetworkReply *networkReply);

    JourneyResultList *lastJourneyResultList;

private:
    enum transportModePreset {
        ALL_TRANSPORT_MODES,
        TRAIN_PUB_TRANS_NOT_EXP_BUS,
        EXP_BUS_PUB_TRANS_NOT_TRAIN
    };

    struct {
        QDateTime dateTime;
        QDateTime firstOption;
        QDateTime lastOption;
        Station from;
        Station via;
        Station to;
        int restrictions;
        Mode mode;
    } lastJourneySearch;

    QString lastStationSearch;
    const QString timetableAPIKey;
    const QString journeyAPIKey;
    const QString timetableBaseURL;
    const QString journeyBaseURL;
    const QString realtimeTimetableBaseURL;
    const QString timetableAPIVersion;
    const QString journeyAPIVersion;
    const int nearbyRadius; // Define what is "nearby" in meters
    const int timetableSpan; // Minutes (valid values: 30 or 120)
    bool realtime;
    QHash<QString, JourneyDetailResultList*> cachedResults;
    // Keep track of the number of "earlier"/"later" searches we did without getting any new results
    int numberOfUnsuccessfulEarlierSearches;
    int numberOfUnsuccessfulLaterSearches;
    QHash<QString, QString> remarkStrings;
    QHash<QString, QString> transportModeStrings;

    virtual void internalSearchJourney(const Station &departureStation, const Station &viaStation,
                                       const Station &arrivalStation, const QDateTime &dateTime,
                                       ParserAbstract::Mode mode, int trainrestrictions);
    QList<JourneyDetailResultItem*> parseJourneySegments(const QVariantMap &journeyData);
    QVariantList ensureList(const QVariant &variable);
    QString translateRemark(const QString& original);
    QString translateTransportMode(QString original);
};

#endif // PARSER_RESROBOT_H
