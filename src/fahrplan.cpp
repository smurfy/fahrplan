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

#include "fahrplan.h"
#include "fahrplan_parser_thread.h"
#include "fahrplan_backend_manager.h"
#include "calendarthreadwrapper.h"
#include "models/favorites.h"
#include "models/stationsearchresults.h"
#include "models/timetable.h"
#include "models/trainrestrictions.h"

#include <QThread>

FahrplanBackendManager *Fahrplan::m_parser_manager = NULL;
StationSearchResults *Fahrplan::m_stationSearchResults= NULL;
Favorites *Fahrplan::m_favorites = NULL;
Timetable *Fahrplan::m_timetable = NULL;
Trainrestrictions *Fahrplan::m_trainrestrictions = NULL;

Fahrplan::Fahrplan(QObject *parent)
    : QObject(parent)
    , m_departureStation(Station(false))
    , m_viaStation(Station(false))
    , m_arrivalStation(Station(false))
    , m_trainrestriction(0)
    , m_mode(DepartureMode)
    , m_dateTime(QDateTime::currentDateTime())
{
    settings = new QSettings(FAHRPLAN_SETTINGS_NAMESPACE, "fahrplan2", this);
    setMode(static_cast<Mode>(settings->value("mode", DepartureMode).toInt()));

    if (!m_parser_manager) {
        int currentBackend = settings->value("currentBackend", 0).toInt();
        m_parser_manager = new FahrplanBackendManager(currentBackend, this);
    }
    connect(m_parser_manager, SIGNAL(parserChanged(const QString &, int)), this, SLOT(onParserChanged(const QString &, int)));

    if (!m_favorites) {
        m_favorites = new Favorites(this);
    }
    connect(m_favorites, SIGNAL(stationSelected(Fahrplan::StationType,Station))
            , SLOT(setStation(Fahrplan::StationType,Station)));

    if (!m_stationSearchResults) {
        m_stationSearchResults = new StationSearchResults(this);
    }
    connect(m_stationSearchResults, SIGNAL(stationSelected(Fahrplan::StationType,Station))
            , SLOT(setStation(Fahrplan::StationType,Station)));

    if (!m_timetable) {
        m_timetable = new Timetable(this);
    }

    if (!m_trainrestrictions) {
        m_trainrestrictions = new Trainrestrictions(this);
    }
}

Fahrplan::~Fahrplan()
{
    qDebug() << this;

    if (m_trainrestrictions) {
        delete m_trainrestrictions;
    }

    if (m_timetable) {
        delete m_timetable;
    }

    if (m_stationSearchResults) {
        disconnect(m_stationSearchResults, SIGNAL(stationSelected(Fahrplan::StationType,Station)),
                   this, SLOT(setStation(Fahrplan::StationType,Station)));
        delete m_stationSearchResults;
    }

    if (m_favorites) {
        disconnect(m_favorites, SIGNAL(stationSelected(Fahrplan::StationType,Station)),
                   this, SLOT(setStation(Fahrplan::StationType,Station)));
        delete m_favorites;
    }

    if (m_parser_manager) {
        unbindParserSignals();
        disconnect(m_parser_manager, SIGNAL(parserChanged(const QString &, int)), this, SLOT(onParserChanged(const QString &, int)));
        delete m_parser_manager;
    }

    if (settings) {
        delete settings;
    }
}

void Fahrplan::bindParserSignals()
{
    if (m_parser_manager->getParser()) {
        connect(m_parser_manager->getParser(), SIGNAL(stationsResult(StationsList)), this, SLOT(onStationSearchResults(StationsList)));
        connect(m_parser_manager->getParser(), SIGNAL(journeyResult(JourneyResultList*)), this, SIGNAL(parserJourneyResult(JourneyResultList*)));
        connect(m_parser_manager->getParser(), SIGNAL(errorOccured(QString)), this, SIGNAL(parserErrorOccured(QString)));
        connect(m_parser_manager->getParser(), SIGNAL(journeyDetailsResult(JourneyDetailResultList*)), this, SIGNAL(parserJourneyDetailsResult(JourneyDetailResultList*)));
        connect(m_parser_manager->getParser(), SIGNAL(timeTableResult(TimetableEntriesList)), this, SLOT(onTimetableResult(TimetableEntriesList)));
    }
}

void Fahrplan::unbindParserSignals()
{
    if (m_parser_manager->getParser()) {
        disconnect(m_parser_manager->getParser(), SIGNAL(stationsResult(StationsList)), this, SLOT(onStationSearchResults(StationsList)));
        disconnect(m_parser_manager->getParser(), SIGNAL(journeyResult(JourneyResultList*)), this, SIGNAL(parserJourneyResult(JourneyResultList*)));
        disconnect(m_parser_manager->getParser(), SIGNAL(errorOccured(QString)), this, SIGNAL(parserErrorOccured(QString)));
        disconnect(m_parser_manager->getParser(), SIGNAL(journeyDetailsResult(JourneyDetailResultList*)), this, SIGNAL(parserJourneyDetailsResult(JourneyDetailResultList*)));
        disconnect(m_parser_manager->getParser(), SIGNAL(timeTableResult(TimetableEntriesList)), this, SLOT(onTimetableResult(TimetableEntriesList)));
    }
}

void Fahrplan::storeSettingsValue(const QString &key, const QString &value)
{
    settings->setValue(key, value);
}

QString Fahrplan::getSettingsValue(const QString &key, const QString &defaultValue)
{
    return settings->value(key, defaultValue).toString();
}

FahrplanParserThread* Fahrplan::parser()
{
    return m_parser_manager->getParser();
}

Favorites* Fahrplan::favorites() const
{
    return m_favorites;
}

QString Fahrplan::getVersion()
{
    return FAHRPLAN_VERSION;
}

bool Fahrplan::supportsCalendar()
{
#if defined(BUILD_FOR_BLACKBERRY) || defined(BUILD_FOR_HARMATTAN) || defined(BUILD_FOR_MAEMO_5) || defined(BUILD_FOR_SYMBIAN) || (defined(BUILD_FOR_OPENREPOS) && defined(BUILD_FOR_SAILFISHOS))
    return true;
#else
    return false;
#endif
}

StationSearchResults *Fahrplan::stationSearchResults() const
{
    return m_stationSearchResults;
}

Timetable *Fahrplan::timetable() const
{
    return m_timetable;
}

Trainrestrictions *Fahrplan::trainrestrictions() const
{
    return m_trainrestrictions;
}


QString Fahrplan::departureStationName() const
{
    if (m_departureStation.valid)
        return m_departureStation.name;
    else
        return tr("please select");
}

QString Fahrplan::viaStationName() const
{
    if (m_viaStation.valid)
        return m_viaStation.name;
    else
        return tr("please select");
}

QString Fahrplan::arrivalStationName() const
{
    if (m_arrivalStation.valid)
        return m_arrivalStation.name;
    else
        return tr("please select");
}

QString Fahrplan::currentStationName() const
{
    if (m_currentStation.valid)
        return m_currentStation.name;
    else
        return tr("please select");
}

QString Fahrplan::directionStationName() const
{
    if (m_directionStation.valid)
        return m_directionStation.name;
    else
        return tr("please select");
}

Fahrplan::Mode Fahrplan::mode() const
{
    return m_mode;
}

void Fahrplan::setMode(Fahrplan::Mode mode)
{
    if (mode == m_mode)
        return;

    m_mode = mode;
    emit modeChanged();

    settings->setValue("mode", static_cast<int>(m_mode));
}

QDateTime Fahrplan::dateTime() const
{
    return m_dateTime;
}

void Fahrplan::setDateTime(const QDateTime &dateTime)
{
    if (dateTime == m_dateTime)
        return;

    m_dateTime = dateTime;
    emit dateTimeChanged();
}

bool Fahrplan::timeFormat24h() const
{
    // A hacky way to detect whether locale uses 24 or 12 hour time format.
    return !QLocale().timeFormat().contains("ap", Qt::CaseInsensitive);
}

void Fahrplan::setStation(Fahrplan::StationType type, const Station &station)
{
    switch (type) {
    case DepartureStation:
        m_departureStation = station;
        emit departureStationChanged();
        saveStationToSettings("departureStation", station);
        break;
    case ViaStation:
        m_viaStation = station;
        emit viaStationChanged();
        saveStationToSettings("viaStation", station);
        break;
    case ArrivalStation:
        m_arrivalStation = station;
        emit arrivalStationChanged();
        saveStationToSettings("arrivalStation", station);
        break;
    case CurrentStation:
        m_currentStation = station;
        emit currentStationChanged();
        saveStationToSettings("currentStation", station);
        break;
    case DirectionStation:
        m_directionStation = station;
        emit directionStationChanged();
        saveStationToSettings("directionStation", station);
        break;
    }
}

void Fahrplan::swapStations(StationType type1, StationType type2)
{
    if (type1 == type2)
        return;
    Station tmp = getStation(type1);
    setStation(type1, getStation(type2));
    setStation(type2, tmp);
}

void Fahrplan::resetStation(StationType type)
{
    setStation(type, Station(false));
}

void Fahrplan::findStationsByName(const QString &stationName)
{
    m_stationSearchResults->setStationsList(StationsList());
    m_parser_manager->getParser()->findStationsByName(stationName);
}

void Fahrplan::findStationsByCoordinates(qreal longitude, qreal latitude)
{
    m_stationSearchResults->setStationsList(StationsList());
    m_parser_manager->getParser()->findStationsByCoordinates(longitude, latitude);
}

void Fahrplan::searchJourney()
{
    ParserAbstract::Mode mode;

    if (m_mode == NowMode) {
        setDateTime(QDateTime::currentDateTime());
        mode = ParserAbstract::Departure;
    } else {
        mode = ParserAbstract::Mode(m_mode);
    }

    m_parser_manager->getParser()->searchJourney(m_departureStation, m_viaStation, m_arrivalStation, m_dateTime, mode, m_trainrestriction);
}

void Fahrplan::getTimeTable()
{
    ParserAbstract::Mode mode;

    if (m_mode == NowMode) {
        setDateTime(QDateTime::currentDateTime());
        mode = ParserAbstract::Departure;
    } else {
        mode = ParserAbstract::Mode(m_mode);
    }

    m_parser_manager->getParser()->getTimeTableForStation(m_currentStation, m_directionStation, m_dateTime, mode, m_trainrestriction);
}

void Fahrplan::setTrainrestriction(int index)
{
    if (index < m_trainrestrictions->count()) {
        m_trainrestriction = index;
    } else {
        m_trainrestriction = 0;
    }
    emit trainrestrictionChanged();
}

QString Fahrplan::trainrestrictionName() const
{
    return m_trainrestrictions->get(m_trainrestriction);
}

void Fahrplan::onParserChanged(const QString &name, int index)
{
    //We need to reconnect all Signals to the new Parser
    bindParserSignals();
    m_stationSearchResults->setStationsList(StationsList());
    loadStations();
    if (m_favorites)
        m_favorites->reload();

    if (m_trainrestrictions) {
        QStringList list = parser()->getTrainRestrictions();
        m_trainrestrictions->setStringList(list);
        setTrainrestriction(0);
    }

    emit parserChanged(name, index);
}

void Fahrplan::onStationSearchResults(const StationsList &result)
{
    m_stationSearchResults->setStationsList(result);

    emit parserStationsResult();
}

void Fahrplan::onTimetableResult(const TimetableEntriesList &timetableEntries)
{
    m_timetable->setTimetableEntries(timetableEntries);

    emit parserTimeTableResult();
}

QString Fahrplan::parserName() const
{
    return m_parser_manager->getParser()->name();
}

QString Fahrplan::parserShortName() const
{
    return m_parser_manager->getParser()->shortName();
}

QStringList Fahrplan::getParserList()
{
    return m_parser_manager->getParserList();
}

void Fahrplan::setParser(int index)
{
    settings->setValue("currentBackend", index);
    m_parser_manager->setParser(index);
}

void Fahrplan::addJourneyDetailResultToCalendar(JourneyDetailResultList *result)
{
    QThread *workerThread = new QThread(this);
    CalendarThreadWrapper *wrapper = new CalendarThreadWrapper(result);

    connect(workerThread, SIGNAL(started()), wrapper, SLOT(addToCalendar()));
    connect(workerThread, SIGNAL(finished()), wrapper, SLOT(deleteLater()));
    connect(workerThread, SIGNAL(finished()), workerThread, SLOT(deleteLater()));

    connect(wrapper,SIGNAL(addCalendarEntryComplete(bool)), SIGNAL(addCalendarEntryComplete(bool)));

    wrapper->moveToThread(workerThread);
    workerThread->start();
}

Station Fahrplan::getStation(StationType type) const
{
    switch (type) {
    case DepartureStation:
        return m_departureStation;
    case ViaStation:
        return m_viaStation;
    case ArrivalStation:
        return m_arrivalStation;
    case CurrentStation:
        return m_currentStation;
    case DirectionStation:
        return m_directionStation;
    default:
        return Station(false);
    }
}

void Fahrplan::loadStations()
{
    setStation(DepartureStation, loadStationFromSettigns("departureStation"));
    setStation(ViaStation, loadStationFromSettigns("viaStation"));
    setStation(ArrivalStation, loadStationFromSettigns("arrivalStation"));
    setStation(CurrentStation, loadStationFromSettigns("currentStation"));
    setStation(DirectionStation, loadStationFromSettigns("directionStation"));
}

void Fahrplan::saveStationToSettings(const QString &key, const Station &station)
{
    settings->beginGroup(m_parser_manager->getParser()->uid());

    if (!station.valid) {
        settings->remove(key);
        settings->endGroup();
        return;
    }

    settings->beginGroup(key);
    settings->setValue("id", station.id);
    settings->setValue("name", station.name);
    settings->endGroup(); // key

    settings->endGroup(); // Parser UID
}

Station Fahrplan::loadStationFromSettigns(const QString &key)
{
    Station station(false);

    settings->beginGroup(m_parser_manager->getParser()->uid());
    settings->beginGroup(key);

    station.id = settings->value("id");
    if (station.id.isValid()) {
        station.name = settings->value("name").toString();
        if (!station.name.isEmpty())
            station.valid = true;
    }

    settings->endGroup(); // key
    settings->endGroup(); // Parser UID

    return station;
}
