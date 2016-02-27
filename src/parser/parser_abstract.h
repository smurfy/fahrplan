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

#ifndef PARSER_ABSTRACT_H
#define PARSER_ABSTRACT_H

#include <QObject>
#include <QStringList>
#include "parser_definitions.h"

class QNetworkAccessManager;
class QNetworkReply;
class QTimer;
class QUrl;
class ParserAbstract : public QObject
{
    Q_OBJECT
    Q_ENUMS(Mode)

public:
    enum Mode { Departure = 0, Arrival = 1 };

    explicit ParserAbstract(QObject *parent = 0);
    ~ParserAbstract();

    static QString getName() { return "Abstract"; }
    virtual QString name() { return getName(); }
    virtual QString shortName() { return getName(); }
    virtual QString uid() { return metaObject()->className(); }

public slots:
    virtual void getTimeTableForStation(const Station &currentStation, const Station &directionStation, const QDateTime &dateTtime, ParserAbstract::Mode mode, int trainrestrictions);
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
    virtual QStringList getTrainRestrictions();
    void cancelRequest();

signals:
    void stationsResult(const StationsList &result);
    void journeyResult(JourneyResultList *result);
    void journeyDetailsResult(JourneyDetailResultList *result);
    void timetableResult(const TimetableEntriesList &timetableEntries);
    void errorOccured(QString msg);

protected slots:
    void networkReplyFinished(QNetworkReply*);
    void networkReplyDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void networkReplyTimedOut();

protected:
    QString userAgent;
    QNetworkAccessManager *NetworkManager;
    FahrplanNS::curReqStates currentRequestState;
    QNetworkReply *lastRequest;
    QTimer *requestTimeout;
    QByteArray acceptEncoding;

    virtual void parseTimeTable(QNetworkReply *networkReply);
    virtual void parseStationsByName(QNetworkReply *networkReply);
    virtual void parseStationsByCoordinates(QNetworkReply *networkReply);
    virtual void parseSearchJourney(QNetworkReply *networkReply);
    virtual void parseSearchLaterJourney(QNetworkReply *networkReply);
    virtual void parseSearchEarlierJourney(QNetworkReply *networkReply);
    virtual void parseJourneyDetails(QNetworkReply *networkReply);
    void sendHttpRequest(QUrl url, QByteArray data, const QList<QPair<QByteArray,QByteArray> > &additionalHeaders = QList<QPair<QByteArray,QByteArray> >());
    void sendHttpRequest(QUrl url);
    QVariantMap parseJson(const QByteArray &data) const;
    QByteArray gzipDecompress(QByteArray compressData);
};

#endif // PARSER_ABSTRACT_H
