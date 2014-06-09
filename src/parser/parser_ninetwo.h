#ifndef PARSER_NINETWO_H
#define PARSER_NINETWO_H

#include "parser_abstract.h"
#include <QUrlQuery>
#include <QUrl>
#include <QNetworkReply>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>

class ParserNinetwo : public ParserAbstract
{
    Q_OBJECT
public:
    ParserNinetwo(QObject* parent = 0);

    // ParserAbstract interface
public:
    QString name(){return "9292ov.nl";}
    static QString getName() { return "9292ov.nl"; }
public slots:
    void getTimeTableForStation(Station &currentStation, Station &directionStation, QDateTime &dateTtime, ParserAbstract::Mode mode, int trainrestrictions);
    void findStationsByName(const QString &stationName);
    void findStationsByCoordinates(qreal longitude, qreal latitude);
    void searchJourney(const Station &departureStation,const Station &viaStation,const Station &arrivalStation,const QDateTime &dateTime,const ParserAbstract::Mode mode, int trainrestrictions);
    void searchJourneyLater();
    void searchJourneyEarlier();
    void getJourneyDetails(const QString &id);
    bool supportsGps();
    bool supportsVia();
    bool supportsTimeTable();
    bool supportsTimeTableDirection();
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
    void parseJourneyOption(QJsonObject object);
};

#endif // PARSER_NINETWO_H
