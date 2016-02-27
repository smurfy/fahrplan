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
    ~ParserXmlVasttrafikSe();

    static QString getName() { return QString("%1 (vasttrafik.se)").arg(tr("Sweden")); }
    virtual QString name() { return getName(); }
    virtual QString shortName() { return "vasttrafik.se"; }

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

    void sendHttpRequestWithBearer(const QUrl &uri);

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

    struct {
        bool isValid;
        QString stationName;
    } m_stationByNameParameters;

    struct {
        bool isValid;
        qreal longitude, latitude;
    } m_stationByCoordinatesParameters;

    static const QString baseRestUrl;
    static const char *consumerCredentials;
    QNetworkAccessManager *m_nam;
    QDateTime m_accessTokenExpiration;
    QString m_accessToken, m_deviceId;

    QDateTime m_earliestArrival, m_latestResultDeparture;

    inline QString i18nConnectionType(const QString &swedishText) const;

    void requestNewAccessToken();

private slots:
    void accessTokenRequestFinished();
};

#endif // PARSER_XMLVASTTRAFIKSE_H
