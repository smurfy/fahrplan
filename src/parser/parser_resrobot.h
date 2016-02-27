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
#ifdef BUILD_FOR_QT5
    #include <QUrlQuery>
#endif

/*
 * Some info on this API:
 *   * ResRobot enables searching for public transport connections in all of Sweden.
 *   * This backend actually uses two API's: "ResRobot - Reseplanerare" and "ResRobot - Stolptidtabeller 2".
 *   * Results are in XML or JSON.
 *   * ResRobot web frontend: http://resrobot.se/
 *   * More API info: https://www.trafiklab.se/api/resrobot-reseplanerare
 *                    https://www.trafiklab.se/api/resrobot-stolptidtabeller-2
 *   * Backend project: https://www.trafiklab.se/projekt/fahrplan
 *   * Report API issues at https://kundo.se/org/trafiklabse/
 *
 * Issues and limitations:
 *   * The transport mode codes aren't reliable. The API can return stuff that isn't documented.
 */

class ParserResRobot : public ParserAbstract
{
    Q_OBJECT
public:
    explicit ParserResRobot(QObject *parent = 0);

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
    enum TransportModePreset {
        ALL_TRANSPORT_MODES,
        TRAIN_PUB_TRANS_NOT_EXP_BUS,
        EXP_BUS_PUB_TRANS_NOT_TRAIN
    };

    enum Product {
        HIGH_SPEED_TRAIN = 2, // Snabbtåg, Expresståg, Arlanda Express
        REGIONAL_TRAIN = 4,   // Regionaltåg, InterCitytåg
        EXPRESS_BUS = 8,      // Expressbuss, Flygbussar
        LOCAL_TRAIN = 16,     // Lokaltåg, Pågatåg, Öresundståg
        SUBWAY = 32,          // Tunnelbana
        TRAM = 64,            // Spårvagn
        BUS = 128,            // Buss
        FERRY = 256           // Färja, Utrikes Färja
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
    QString searchEarlierReference;
    QString searchLaterReference;
    Mode timetableSearchMode;
    const QString timetableAPIKey;
    const QString journeyAPIKey;
    const QString baseURL;
    const QString timetableAPIVersion;
    const QString journeyAPIVersion;
    QMap<QString, JourneyDetailResultList*> cachedResults;
    QHash<QString, QString> hafasAttributes;
    QHash<QString, QString> transportModeStrings;

#if defined(BUILD_FOR_QT5)
    virtual void doSearchJourney(QUrlQuery query);
#else
    virtual void doSearchJourney(QUrl query);
#endif
    QList<JourneyDetailResultItem*> parseJourneySegments(const QVariantMap &journeyData);
    QString hafasAttribute(const QString& original);
    QString translateTransportMode(QString original);
    QString formatRestrictions(int restriction);
};

#endif // PARSER_RESROBOT_H
