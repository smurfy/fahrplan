#ifndef PARSER_SEARCH_CH_H
#define PARSER_SEARCH_CH_H

#include <QNetworkReply>
#include <QList>
#include <QMap>

#include "parser_abstract.h"

namespace parser_search_ch
{
    class TimetableRow : public QObject
    {
        Q_OBJECT

        public:
            TimetableRow(const QVariantMap& stop);
            void load(const QVariantMap& departure);
            void appendTo(TimetableEntriesList& tt);
        private:
            const QVariantMap& stop;
            void loadTrainType(const QVariantMap& departure);
            void loadTrainTypeWithoutLine(const QVariant& trainType);
            void loadDelay(const QVariantMap& departure);
            void loadDeparturePlatform(const QVariantMap&);

            TimetableEntry timetable;
    };

    class TrainTypeList : public QObject
    {
        Q_OBJECT

        public:
            TrainTypeList();
            void append(const QString&);
            QString toString();
        private:
            void appendUnique(const QString&);
            QStringList types;
    };

    class JourneySearch
    {
        public:
            Station from;
            Station via;
            Station to;
            QDateTime when;
            ParserAbstract::Mode mode;
            int restrictions;
    };

    class JourneySearchResult : public JourneyResultList
    {
        public:
            JourneySearchResult(const JourneySearch&);
        private:
            void loadTimeInfo(const QDateTime&, const ParserAbstract::Mode&);
    };

    class JourneyConnection : public JourneyResultItem
    {
        public:
            JourneyConnection(int id);
            void load(const QVariantMap&);
        private:
            QDateTime departure;
            QDateTime arrival;
            QVariantList legs;

            void loadDepartureTime(const QDateTime&, const QVariant& delay);
            void loadArrivalTime(const QDateTime&, const QVariant& delay);
            void loadDuration(const QVariant&);
            void loadTrainTypes(const QVariantList&);
            void countTransfers(const QVariantList&);
            void checkIfCancelled(const QVariant& dep_delay);
            friend class JourneyConnectionDetails;
    };

    class JourneyConnectionDetails : public JourneyDetailResultList
    {
        public:
            JourneyConnectionDetails(JourneyConnection*, const JourneySearch&);
        private:
            void loadSegments(JourneyConnection*);
    };

    class JourneyConnectionTime
    {
        public:
            JourneyConnectionTime(const QDateTime& time, const QVariant& delay);
            QString toString();
        private:
            const QDateTime& ts;
            const QVariant& delay;
    };

    class JourneySegment : public JourneyDetailResultItem
    {
        Q_OBJECT
        public:
            JourneySegment(const QVariantMap& leg, const QVariant& exit);
        private:
            void loadTrainType(const QVariantMap& leg);
            void loadExit(const QVariantMap& leg, const QVariantMap& exit);
            void loadDelay(const QVariantMap& leg);
            void loadDepartureTrack(const QVariantMap& leg);
    };
}

class ParserSearchCH : public ParserAbstract
{
    Q_OBJECT
public:
    explicit ParserSearchCH(QObject *parent=0);
    static QString getName() { return QString("%1 (timetable.search.ch)").arg(tr("Switzerland")); }
    virtual QString name() { return getName(); }
    virtual QString shortName() { return "search.ch"; }
    static QDateTime tsFromMap(const QVariantMap& map, const QString& key);

public slots:
    virtual bool supportsGps();
    virtual bool supportsVia();
    virtual bool supportsTimeTable();
    virtual void findStationsByName(const QString &stationName);
    virtual void findStationsByCoordinates(qreal longitude, qreal latitude);
    virtual void getTimeTableForStation(const Station &currentStation,
                                        const Station &directionStation,
                                        const QDateTime &dateTime,
                                        ParserAbstract::Mode mode,
                                        int trainrestrictions);
    virtual void searchJourney(const Station &departureStation,
                                        const Station &viaStation,
                                        const Station &arrivalStation,
                                        const QDateTime &dateTime,
                                        ParserAbstract::Mode mode,
                                        int trainrestrictions);
    virtual void getJourneyDetails(const QString &id);

protected:
    QStringList getTrainRestrictions();
    virtual void parseStations(QNetworkReply *);
    virtual void parseStationsByName(QNetworkReply *);
    virtual void parseStationsByCoordinates(QNetworkReply *);
    virtual void parseTimeTable(QNetworkReply *);
    virtual void parseSearchJourney(QNetworkReply *);
    virtual void searchJourneyLater();
    virtual void searchJourneyEarlier();
    virtual void sendRequest(QUrl url);

    QList<JourneyDetailResultList*> details;
    parser_search_ch::JourneySearch lastJourneySearch;

private:
    QVariantList parseJsonList(const QByteArray &json) const;
#if defined(BUILD_FOR_QT5)
    void addRestrictionsToQuery(QUrlQuery&, int);
#else
    void addRestrictionsToQuery(QUrl&, int);
#endif
    void parseStationRow(StationsList& results, const QVariantMap& properties);
    void setTTTrainTypeWithoutLine(TimetableEntry&, const QVariant&);
    void setTTDepartureGPSPosition(TimetableEntry&, const QVariantMap&);
    void setTTDeparturePlatform(TimetableEntry&, const QVariantMap&, const QVariantMap&);
    void setTTDelay(TimetableEntry&, const QVariantMap&);
};

#endif
