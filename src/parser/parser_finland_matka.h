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
 *   - Requests are made as HTTP GET requests, and responses are delivered as XML.
 *   - Web frontend: http://matka.fi, http://journey.fi or http://resa.fi
 *   - More API info: http://developer.matka.fi/pages/en/http-get-interface.php
 *
 * Issues and limitations:
 *   - Responses are very limited. Some examples:
 *      - Route responses are missing city name (only a city code is returned), transport
 *        mode (again, only a code is returned), line direction and end station, operator
 *        in human readable format, travel info (available services, limitations etc)
 *      - Timetable responses lack proper information about line direction, final station,
 *        platform, etc.
 *   - Coordinates have to be in the Finnish KKJ3 coordinate system rather than WGS 84.
 *   - Stops/places don't have unambiguous IDs, and instead of searching for example
 *     a route between two stop IDs, the coordinates have to be used.
 *   - Strange route segments are returned, like 0.912 meters of walk.
 *   - There is no way to search for earlier or later journeys relative a previous search.
 *   - Although the X and Y axis are northing and easting, respectively, for KKJ3 (at least
 *     according to http://spatialreference.org/ref/epsg/2393/), matka.fi seems to use them
 *     the other way around.
 */

#ifndef PARSER_FINLANDMATKA_H
#define PARSER_FINLANDMATKA_H

#include <QNetworkReply>
#include <QDomNode>
#include <QList>
#include <QMap>

#include "parser_abstract.h"
#include "../3rdparty/gauss-kruger-cpp/gausskruger.h"

class KKJ3 : public gausskruger::Projection
{
public:
    double flattening() { return 1 / 297.0; }
    double equatorialRadius() { return 6378388.0; }
    double centralMeridian() { return 27.0; }
    double scale() { return 1.0; }
    double falseNorthing() { return 0.0; }
    double falseEasting() { return 3500000.0; }
};

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
    // Found using http://api.matka.fi/timetables/?m=ttype&user=<user>&pass=<password>
    enum TransportType {
        BusTKL1 = 1,
        ExpressTrain = 2,
        Tolstoi = 3,
        Allegro = 4,
        InterCity = 5,
        InterCity2 = 6,
        RegionalTrain = 7,
        TrainBus = 8,
        ConnectionBus1 = 9,
        ConnectionBus2 = 10,
        PohjolanLiikenne = 11,
        CommuterTrain1 = 12,
        CommuterTrain2 = 13,
        Pendolino = 14,
        BusVRVakio = 15,
        BusVRPika = 16,
        BusVRExpress = 17,
        Taxi = 18,
        BusSHJoe = 19,
        BusSHMKL = 20,
        BusSHLPR = 21,
        BusSHOul = 22,
        BusSHVaa = 23,
        BusSHKot = 24,
        BusTur1 = 25,
        Air = 26,
        BusValluExpress = 27,
        BusValluPika = 28,
        BusValluVakio1 = 29,
        CarValluVakio = 30,
        BusValluVakio2 = 31,
        BusValluLinjausok1 = 32,
        CarLinjausok = 33,
        BusValluLinjausok2 = 34,
        BusYTV1 = 35,
        Tram = 36,
        BusYTV2 = 37,
        BusYTV3 = 38,
        BusYTV4 = 39,
        Metro = 40,
        Ferry = 41,
        BusYTV5 = 42,
        BusYTV6 = 43,
        BusYTV7 = 44,
        BusYTV8 = 45,
        TrainYTV1 = 46,
        TrainYTV2 = 47,
        BusYTV9 = 48,
        BusYTV10 = 49,
        BusYTV11 = 50,
        BusYTV12 = 51,
        BusYTV13 = 52,
        KirkkonummiCityBus = 53,
        SipooCityBus = 54,
        KeravaCityBus = 55,
        BusOul = 56,
        BusTKL2 = 57,
        BusTKLVakio = 58,
        BusJKL = 59,
        BusSHKuo = 60,
        BusTur2 = 61,
        BusSK = 62,
        BusRae = 63,
        BusKaj = 64,
        BusKVL = 65
    };

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

    enum JourneySegmentParseResult {
        JourneySegmentOK,
        SkipJourneySegment,
        SkipJourney
    };

    enum TimetableRequestType {
        TimetableAreaRequest,
        TimetableStopRequest,
        TimetableLineRequest,
        NoTimetableRequest
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
        Mode mode;
    } lastJourneySearch;

    struct {
        Station currentStation;
        Station directionStation;
        QDateTime dateTime;
        Mode mode;
        int restrictions;
        QSet<QString> stopsMatchingCurrentStation;
    } lastTimetableSearch;

    struct SortableTimetableEntry {
        TimetableEntry entry;
        QDateTime dateTime;
        QString lineID;
    };

    struct SortUtility {
        bool operator()(const Station& s1, const Station& s2);
        bool operator()(const SortableTimetableEntry& e1, const SortableTimetableEntry& e2);
    } sortUtility;

    QMap<QString, JourneyDetailResultList*> cachedResults; // journey ID => detailed journey results
    // Keep track of the number of "earlier"/"later" searches we did without getting any new results
    int numberOfUnsuccessfulEarlierSearches;
    int numberOfUnsuccessfulLaterSearches;
    QStringList timetableStopsToDownload;
    QSet<QString> linesWaitingForDownload;
    QMap<QString, QStringList> lineStops; // line ID => stop IDs for stops along line
    QMap<QString, QString> stopNames; // stop ID => stop name
    QList<SortableTimetableEntry> timetable;
    TimetableRequestType timetableRequestState;
    const QString baseURL;
    const QString username;
    const QString password;
    QMap<int, QString> cityNames; // city ID => city name
    QMap<QString, QString> finnishToSwedishCityNames;
    KKJ3 projection;

    void internalSearchJourney(const Station &departureStation, const Station &viaStation,
                                       const Station &arrivalStation, const QDateTime &dateTime,
                                       Mode mode, int trainrestrictions);
    bool parseTimetableAreaInfo(QNetworkReply *networkReply);
    bool parseTimetableStopInfo(QNetworkReply *networkReply);
    bool parseTimetableLineInfo(QNetworkReply *networkReply);
    void finishTimetable();
    inline QString getAttribute(const QDomNode& node, const QString& key) {
        return node.attributes().namedItem(key).toAttr().value();
    }
    QList<JourneyDetailResultItem*> parseJourneySegments(const QDomElement& journeyData, double& totalWalkingDistance);
    JourneySegmentParseResult parseJourneySegment(const QDomElement& segmentData,
                                                  bool isWalk, double& distance,
                                                  JourneyDetailResultItem* journeySegment);
    QString prettyStationType(const QString& type);
    QString placeName(const QDomElement& place, bool localize=true);
    QString timetableStopName(const QDomElement& stopElement, bool localize=true);
    QString cityName(const QDomElement& element, bool localize=true);
    QString cityName(int code, bool localize=true);
    void localizeCityName(QString& placeName);
    static QString formatDistance(double distance);
    static QByteArray toPercentEncoding(const QString& string);
    QString transportTypeName(int code);
    bool transportTypeIsSelected(int transportType, int selectedTransportModeFilter);
    void setupCityNames();
};

#endif // PARSER_FINLANDMATKA_H
