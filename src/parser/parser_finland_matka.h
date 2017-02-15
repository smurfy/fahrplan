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

/*
 * Some info on the matka.fi API:
 *   - matka.fi enables searching for public transport connections in all of Finland.
 *   - The current API is replacing the former HTTP GET API, which was announced to be
 *     shut down at the end of 2016.
 *   - The API is divided into different sub APIs. This Fahrplan backend is using
 *      - Geocoding: HTTP GET (for finding stops etc. by name or position)
 *      - Routing: GraphQL (for finding routes and timetables).
 *   - Status of the new service (February 2017):
 *      - The old web frontend http://matka.fi redirects to the Finnish Transport Agency
 *      - It's not really obvious how the new service is going to be branded. It's
 *        documented at https://digitransit.fi/, but points at https://beta.digitransit.fi/
 *        for web access. That website labels itself at "the new matka.fi". The new
 *        web frontend is also available at https://beta.matka.fi.
 *   - More API info: https://www.digitransit.fi/en/developers/services-and-apis/
 *
 * Issues and limitations:
 *
 * Although the new API is better than the old, it's still not so good.
 *   - The geocoding and routing APIs aren't well connected. While the geocoding API
 *     returns one kind of node IDs, the routing API is using coordinates for searching for
 *     journeys but IDs for timetables. Since the IDs used in the routing API and for
 *     searching for timetables are not exactly the same as returned by the geocoding API,
 *     some tweaking of them is necessary to find any timetables.
 *   - Route responses are missing city name.
 *   - Not possible to search for timetable arrivals.
 *   - Journey and timetable search is always referring to specific platforms of the stops,
 *     i.e. if you don't know which of the platform the bus departs from, the journey will
 *     start with a walk to the plaform from were the bus departs.
 *   - There is no way to search for earlier or later journeys relative a previous search.
 */

#ifndef PARSER_FINLANDMATKA_H
#define PARSER_FINLANDMATKA_H

#include <QNetworkReply>
#include <QList>
#include <QMap>

#include "parser_abstract.h"

class ParserFinlandMatka : public ParserAbstract
{
    Q_OBJECT
public:
    explicit ParserFinlandMatka(QObject *parent=0);
    static QString getName() { return QString("%1 (matka.fi)").arg(tr("Finland")); }
    virtual QString name() { return getName(); }
    virtual QString shortName() { return "matka.fi"; }

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
                                        const QDateTime &dateTime,
                                        ParserAbstract::Mode mode,
                                        int trainrestrictions);
    virtual void searchJourney(const Station &departureStation, const Station &viaStation,
                               const Station &arrivalStation, const QDateTime &dateTime,
                               ParserAbstract::Mode mode, int trainrestrictions);
    virtual void searchJourneyLater();
    virtual void searchJourneyEarlier();
    virtual void getJourneyDetails(const QString &id);

protected:
    virtual void parseTimeTable(QNetworkReply *networkReply);
    virtual void parseStationsByName(QNetworkReply *networkReply);
    virtual void parseStationsByCoordinates(QNetworkReply *networkReply);
    virtual void parseSearchJourney(QNetworkReply *networkReply);
    virtual void parseSearchLaterJourney(QNetworkReply *networkReply);
    virtual void parseSearchEarlierJourney(QNetworkReply *networkReply);
    virtual void parseJourneyDetails(QNetworkReply *networkReply);

private:
    // Note: When changing the enum below, also update the list in getTrainRestrictions()
    enum TransportModeFilter {
        UseAll,
        UseBus,
        UseTrain,
        UseAirplane,
        UseBusAndTrain,
        UseBusAndAirplane,
        UseTrainAndAirplane
    };

    struct {
        QDateTime dateTime;
        QDateTime firstOption;
        QDateTime lastOption;
        int resultCount;
        Station from;
        Station via;
        Station to;
        int restrictions;
        QStringList restrictionStrings;
        Mode mode;
    } lastJourneySearch;

    struct {
        Station currentStation;
        Station directionStation;
        QDateTime dateTime;
        Mode mode;
        int restrictions;
    } lastTimetableSearch;

    QMap<QString, JourneyDetailResultList*> cachedResults; // journey ID => detailed journey results
    // Keep track of the number of "earlier"/"later" searches we did without getting any new results
    int numberOfUnsuccessfulEarlierSearches;
    int numberOfUnsuccessfulLaterSearches;

    void sendRequest(QUrl url, QVariantMap request = QVariantMap());
    void internalSearchJourney(const Station &departureStation, const Station &viaStation,
                                       const Station &arrivalStation, const QDateTime &dateTime,
                                       Mode mode, int trainrestrictions);
    QList<JourneyDetailResultItem*> parseJourneySegments(const QVariantMap& itinerary);
    QString prettyPlaceType(const QString& type);
    static QString formatDistance(double distance);
    QString transportModeName(const QString& mode);
    bool transportModeIsSelected(const QString& mode, int selectedFilter);
    QStringList selectedTransportModes(int selection);
    QString languageCode() const;
    QString parseNodeName(const QVariantMap& node);
    QString timetableStationID(QString stationIDFromSearchResult, Mode mode);
};

#endif // PARSER_FINLANDMATKA_H
