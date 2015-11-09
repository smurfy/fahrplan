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

#ifndef FAHRPLAN_H
#define FAHRPLAN_H

#include "parser/parser_definitions.h"
#include "parser/parser_abstract.h"

#include <QObject>
#include <QSettings>
#include <QStringList>
#include <QStringListModel>

class FahrplanBackendManager;
class FahrplanParserThread;
class StationSearchResults;
class Timetable;
class Favorites;
class Trainrestrictions;
class Fahrplan : public QObject
{
    Q_OBJECT

    Q_PROPERTY(FahrplanParserThread *parser READ parser)
    Q_PROPERTY(QString parserName READ parserName NOTIFY parserChanged)
    Q_PROPERTY(QString parserShortName READ parserShortName NOTIFY parserChanged)
    Q_PROPERTY(QString version READ getVersion CONSTANT)
    Q_PROPERTY(bool supportsCalendar READ supportsCalendar CONSTANT)

    Q_PROPERTY(StationSearchResults *stationSearchResults READ stationSearchResults CONSTANT)
    Q_PROPERTY(Favorites *favorites READ favorites CONSTANT)
    Q_PROPERTY(Timetable *timetable READ timetable CONSTANT)
    Q_PROPERTY(Trainrestrictions *trainrestrictions READ trainrestrictions CONSTANT)
    Q_PROPERTY(QString departureStationName READ departureStationName NOTIFY departureStationChanged)
    Q_PROPERTY(QString viaStationName READ viaStationName NOTIFY viaStationChanged)
    Q_PROPERTY(QString arrivalStationName READ arrivalStationName NOTIFY arrivalStationChanged)
    Q_PROPERTY(QString currentStationName READ currentStationName NOTIFY currentStationChanged)
    Q_PROPERTY(QString directionStationName READ directionStationName NOTIFY directionStationChanged)
    Q_PROPERTY(QString trainrestrictionName READ trainrestrictionName NOTIFY trainrestrictionChanged)

    Q_PROPERTY(Mode mode READ mode WRITE setMode NOTIFY modeChanged)
    Q_PROPERTY(QDateTime dateTime READ dateTime WRITE setDateTime NOTIFY dateTimeChanged)

    Q_ENUMS(StationType)
    Q_ENUMS(Mode)

    public:
        enum StationType {
            DepartureStation,
            ViaStation,
            ArrivalStation,
            CurrentStation,
            DirectionStation
        };

        enum Mode {
            DepartureMode = ParserAbstract::Departure,
            ArrivalMode = ParserAbstract::Arrival,
            NowMode
        };

        explicit Fahrplan(QObject *parent = 0);
        virtual ~Fahrplan();
        FahrplanParserThread *parser();
        Favorites *favorites() const;
        QString parserName() const;
        QString parserShortName() const;
        QString getVersion();
        bool supportsCalendar();

        StationSearchResults *stationSearchResults() const;
        Timetable *timetable() const;
        Trainrestrictions *trainrestrictions() const;
        QString departureStationName() const;
        QString viaStationName() const;
        QString arrivalStationName() const;
        QString currentStationName() const;
        QString directionStationName() const;
        QString trainrestrictionName() const;

        Fahrplan::Mode mode() const;
        void setMode(Fahrplan::Mode mode);

        QDateTime dateTime() const;
        void setDateTime(const QDateTime &dateTime);

        Q_INVOKABLE bool timeFormat24h() const;

    public slots:
        QStringList getParserList();
        void setParser(int index);
        void storeSettingsValue(const QString &key, const QString &value);
        QString getSettingsValue(const QString &key, const QString &defaultValue);
        void swapStations(StationType type1, StationType type2);
        void resetStation(StationType type);
        void findStationsByName(const QString &stationName);
        void findStationsByCoordinates(qreal longitude, qreal latitude);
        void getTimeTable();
        void searchJourney();
        void addJourneyDetailResultToCalendar(JourneyDetailResultList *result);
        void setTrainrestriction(int index);

    signals:
        void departureStationChanged();
        void viaStationChanged();
        void arrivalStationChanged();
        void currentStationChanged();
        void directionStationChanged();
        void trainrestrictionChanged();

        void modeChanged();
        void dateTimeChanged();

        void parserStationsResult();
        void parserJourneyResult(JourneyResultList *result);
        void parserJourneyDetailsResult(JourneyDetailResultList *result);
        void parserTimeTableResult();
        void parserErrorOccured(const QString &msg);
        void parserChanged(const QString &name, int index);
        void addCalendarEntryComplete(bool success);

    private slots:
        void setStation(Fahrplan::StationType type, const Station &station);
        void onParserChanged(const QString &name, int index);
        void onStationSearchResults(const StationsList &result);
        void onTimetableResult(const TimetableEntriesList &timetableEntries);
        void bindParserSignals();
        void unbindParserSignals();

    private:
        static FahrplanBackendManager *m_parser_manager;
        static StationSearchResults *m_stationSearchResults;
        static Favorites *m_favorites;
        static Timetable *m_timetable;
        static Trainrestrictions *m_trainrestrictions;
        QSettings *settings;

        Station m_departureStation;
        Station m_viaStation;
        Station m_arrivalStation;
        Station m_currentStation;
        Station m_directionStation;
        int m_trainrestriction;

        Mode m_mode;
        QDateTime m_dateTime;

        Station getStation(StationType type) const;
        void loadStations();
        void saveStationToSettings(const QString &key, const Station &station);
        Station loadStationFromSettigns(const QString &key);
};
Q_DECLARE_METATYPE(Fahrplan::StationType)
Q_DECLARE_METATYPE(Fahrplan::Mode)

#endif // FAHRPLAN_H
