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

#include <QThread>

FahrplanBackendManager *Fahrplan::m_parser_manager = NULL;
StationSearchResults *Fahrplan::m_stationSearchResults= NULL;
Favorites *Fahrplan::m_favorites_manager = NULL;

Fahrplan::Fahrplan(QObject *parent)
    : QObject(parent)
    , m_departureStation(Station(false))
    , m_viaStation(Station(false))
    , m_arrivalStation(Station(false))
{
    settings = new QSettings("smurfy", "fahrplan2");

    if (!m_parser_manager) {
        int currentBackend = settings->value("currentBackend", 0).toInt();
        m_parser_manager = new FahrplanBackendManager(currentBackend);
    }
    connect(m_parser_manager, SIGNAL(parserChanged(const QString &, int)), this, SLOT(onParserChanged(const QString &, int)));

    if (!m_favorites_manager) {
        m_favorites_manager = new Favorites(this);
    }
    connect(m_favorites_manager, SIGNAL(stationSelected(Fahrplan::StationType,Station))
            , SLOT(selectStation(Fahrplan::StationType,Station)));

    if (!m_stationSearchResults) {
        m_stationSearchResults = new StationSearchResults(this);
    }
    connect(m_stationSearchResults, SIGNAL(stationSelected(Fahrplan::StationType,Station))
            , SLOT(selectStation(Fahrplan::StationType,Station)));

    m_departureStation = loadStationFromSettigns("departureStation");
    m_viaStation = loadStationFromSettigns("viaStation");
    m_arrivalStation = loadStationFromSettigns("arrivalStation");
    m_currentStation = loadStationFromSettigns("currentStation");
    m_directionStation = loadStationFromSettigns("directionStation");
}

void Fahrplan::bindParserSignals()
{
    if (m_parser_manager->getParser()) {
        connect(m_parser_manager->getParser(), SIGNAL(stationsResult(StationsResultList*)), this, SLOT(onStationSearchResults(StationsResultList*)));
        connect(m_parser_manager->getParser(), SIGNAL(journeyResult(JourneyResultList*)), this, SIGNAL(parserJourneyResult(JourneyResultList*)));
        connect(m_parser_manager->getParser(), SIGNAL(errorOccured(QString)), this, SIGNAL(parserErrorOccured(QString)));
        connect(m_parser_manager->getParser(), SIGNAL(journeyDetailsResult(JourneyDetailResultList*)), this, SIGNAL(parserJourneyDetailsResult(JourneyDetailResultList*)));
        connect(m_parser_manager->getParser(), SIGNAL(timeTableResult(TimeTableResultList*)), this, SIGNAL(parserTimeTableResult(TimeTableResultList*)));
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
    return m_favorites_manager;
}

QString Fahrplan::getVersion()
{
    return FAHRPLAN_VERSION;
}

StationSearchResults *Fahrplan::stationSearchResults() const
{
    return m_stationSearchResults;
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

void Fahrplan::selectStation(Fahrplan::StationType type, const Station &station)
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

void Fahrplan::swapStations(int type1, int type2)
{
    Station tmp = getStation(StationType(type1));
    selectStation(StationType(type1), getStation(StationType(type2)));
    selectStation(StationType(type2), tmp);
}

void Fahrplan::resetStation(int type)
{
    selectStation(StationType(type), Station(false));
}

void Fahrplan::searchJourney(const QDate &date, const QTime &time, ParserAbstract::Mode mode, int trainrestrictions)
{
    m_parser_manager->getParser()->searchJourney(m_departureStation, m_viaStation, m_arrivalStation, date, time, mode, trainrestrictions);
}

void Fahrplan::getTimeTable(const QDate &date, const QTime &time, ParserAbstract::Mode mode, int trainrestrictions)
{
    m_parser_manager->getParser()->getTimeTableForStation(m_currentStation, m_directionStation, date, time, mode, trainrestrictions);
}

void Fahrplan::onParserChanged(const QString &name, int index)
{
    //We need to reconnect all Signals to the new Parser
    bindParserSignals();
    emit parserChanged(name, index);
}

void Fahrplan::onStationSearchResults(StationsResultList *list)
{
    StationsList stations;
    Station station;
    StationsResultItem *item;
    for (int k = 0; k < list->itemcount(); k++) {
        item = list->getItem(k);
        station = Station();
        station.id = item->stationId();
        station.name = item->stationName();
        station.type = item->stationType();
        station.miscInfo = item->miscInfo();
        station.latitude = item->latitude();
        station.longitude = item->longitude();
        stations << station;
    }
    m_stationSearchResults->setStationsList(stations);

    emit parserStationsResult();
}

QString Fahrplan::parserName()
{
    return m_parser_manager->getParser()->name();
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

void Fahrplan::saveStationToSettings(const QString &key, const Station &station)
{
    if (!station.valid)
        settings->remove(key);

    settings->beginGroup(key);
    settings->setValue("id", station.id);
    settings->setValue("name", station.name);
    settings->endGroup();
}

Station Fahrplan::loadStationFromSettigns(const QString &key)
{
    Station station(false);

    settings->beginGroup(key);

    station.id = settings->value("id");
    if (station.id.isValid()) {
        station.name = settings->value("name").toString();
        if (!station.name.isEmpty())
            station.valid = true;
    }

    settings->endGroup();

    return station;
}
