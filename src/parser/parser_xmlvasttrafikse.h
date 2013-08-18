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

#ifndef PARSER_XMLVASTTRAFIKSE_H
#define PARSER_XMLVASTTRAFIKSE_H

#include "parser_abstract.h"

class ParserXmlVasttrafikSe : public ParserAbstract
{
    Q_OBJECT

public:
    explicit ParserXmlVasttrafikSe(QObject *parent = 0);
    static QString getName() {
        return "vasttrafik.se";
    }
    QString name() {
        return "vasttrafik.se";
    }

public slots:
    virtual void getTimeTableForStation(const Station &currentStation, const Station &directionStation, const QDateTime &dateTime, Mode mode, int trainrestrictions);
    virtual void findStationsByName(const QString &stationName);
    virtual void findStationsByCoordinates(qreal longitude, qreal latitude);
    virtual void searchJourney(const Station &departureStation, const Station &viaStation, const Station &arrivalStation, const QDateTime &dateTime, ParserAbstract::Mode mode, int trainrestrictions);
    virtual void searchJourneyLater();
    virtual void searchJourneyEarlier();
    virtual void getJourneyDetails(const QString &id);
    virtual bool supportsGps();
    virtual bool supportsVia();
    virtual bool supportsTimeTable();
    virtual bool supportsTimeTableDirection();
//     virtual QStringList getTrainRestrictions();
//     void cancelRequest();

protected:
    virtual void parseStationsByName(QNetworkReply *networkReply);
    virtual void parseStationsByCoordinates(QNetworkReply *networkReply);
    virtual void parseTimeTable(QNetworkReply *networkReply);
    virtual void parseSearchJourney(QNetworkReply *networkReply);

private:
    static const qlonglong TRIP_RTDATA_NONE;
    static const qlonglong TRIP_RTDATA_ONTIME;
    static const qlonglong TRIP_RTDATA_WARNING;

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

    const QString apiKey;
    const QString baseRestUrl;

    QDateTime m_earliestArrival, m_latestResultDeparture;

    inline QString i18nConnectionType(const QString &swedishText) const;
};

#endif // PARSER_XMLVASTTRAFIKSE_H
