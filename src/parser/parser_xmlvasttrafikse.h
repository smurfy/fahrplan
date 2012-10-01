/*******************************************************************************

    This file is a part of Fahrplan for maemo 2009-2012
    Contributed by Thomas Fischer <fischer@unix-ag.uni-kl.de>

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
    virtual void getTimeTableForStation(const QString &stationName, const QString &directionStationName, const QDate &date, const QTime &time, Mode mode, int trainrestrictions);
    virtual void findStationsByName(const QString &stationName);
    virtual void findStationsByCoordinates(qreal longitude, qreal latitude);
    virtual void searchJourney(const QString &departureStation, const QString &arrivalStation, const QString &viaStation, const QDate &date, const QTime &time, Mode mode, int trainrestrictions);
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
    static const qlonglong ERR_UNKNOWN_STATION;
    static const qlonglong ERR_INVALID_STATION;
    static const qlonglong TRIP_RTDATA_NONE;
    static const qlonglong TRIP_RTDATA_ONTIME;
    static const qlonglong TRIP_RTDATA_WARNING;

    struct {
        bool isValid;
        QString departureStation;
        QString arrivalStation;
        QString viaStation;
        QDate date;
        QTime time;
        Mode mode;
        int trainrestrictions;
    } m_searchJourneyParameters;

    struct {
        bool isValid;
        QString stationName;
        QString directionStationName;
        QDate date;
        QTime time;
        Mode mode;
        int trainrestrictions;
    } m_timeTableForStationParameters;

    const QString apiKey;
    const QString baseRestUrl;

    QDateTime m_earliestArrival, m_latestResultDeparture;

    inline QString i18nConnectionType(const QString &swedishText) const;
};

#endif // PARSER_XMLVASTTRAFIKSE_H
