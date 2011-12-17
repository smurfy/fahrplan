/*******************************************************************************

    This file is a part of Fahrplan for maemo 2009-2011

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

#include "fahrplan.h"

FahrplanBackendManager *Fahrplan::m_parser_manager;
FahrplanFavoritesManager *Fahrplan::m_favorites_manager;

Fahrplan::Fahrplan(QObject *parent) :
    QObject(parent)
{
    settings = new QSettings("smurfy", "fahrplan2");

    if (!m_parser_manager) {
        int currentBackend = settings->value("currentBackend", 0).toInt();
        m_parser_manager = new FahrplanBackendManager(currentBackend);
    }

    if (!m_favorites_manager) {
        m_favorites_manager = new FahrplanFavoritesManager();
    }

    connect(m_parser_manager, SIGNAL(parserChanged(QString,int)), this, SLOT(onParserChanged(QString, int)));
    connect(m_favorites_manager, SIGNAL(favoritesChanged(QStringList)), this, SLOT(onFavoritesChanged(QStringList)));
}

void Fahrplan::storeSettingsValue(QString key, QString value)
{
    settings->setValue(key, value);
}

QString Fahrplan::getSettingsValue(QString key, QString defaultValue)
{
    return settings->value(key, defaultValue).toString();
}

ParserAbstract* Fahrplan::parser()
{
    return m_parser_manager->getParser();
}

FahrplanFavoritesManager* Fahrplan::favorites()
{
    return m_favorites_manager;
}

QString Fahrplan::getVersion()
{
    return "2.0.6";
}

void Fahrplan::onFavoritesChanged(QStringList favorites)
{
    favoritesChanged(favorites);
}

void Fahrplan::onParserChanged(QString name, int index)
{
    //We need to reconnect all Signals to the new Parser
    connect(m_parser_manager->getParser(), SIGNAL(stationsResult(StationsResultList*)), this, SLOT(onStationsResult(StationsResultList*)));
    connect(m_parser_manager->getParser(), SIGNAL(journeyResult(JourneyResultList*)), this, SLOT(onJourneyResult(JourneyResultList*)));
    connect(m_parser_manager->getParser(), SIGNAL(errorOccured(QString)), this, SLOT(onErrorOccured(QString)));
    connect(m_parser_manager->getParser(), SIGNAL(journeyDetailsResult(JourneyDetailResultList*)), this, SLOT(onJourneyDetailsResult(JourneyDetailResultList*)));
    connect(m_parser_manager->getParser(), SIGNAL(timeTableResult(TimeTableResultList*)), this, SLOT(onTimeTableResult(TimeTableResultList*)));

    emit parserChanged(name, index);
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

bool Fahrplan::addJourneyDetailResultToCalendar(JourneyDetailResultList *result)
{
    QString desc;
    desc.append(result->departureStation() + " to " + result->arrivalStation() + "\n");

    if (!result->info().isEmpty()) {
        desc.append(result->info() + "\n");
    }

    desc.append("Duration: " + result->duration() + "\n\n");

    for (int i=0; i < result->itemcount(); i++) {
        JourneyDetailResultItem *item = result->getItem(i);

        if (!item->train().isEmpty()) {
            desc.append(item->train() + "\n");
        }
        desc.append(item->departureDateTime().date().toString() + " " + item->departureDateTime().time().toString("HH:mm") + " " + item->departureStation());
        if (!item->departureInfo().isEmpty()) {
            desc.append(" - " + item->departureInfo());
        }
        desc.append("\n");
        desc.append(item->arrivalDateTime().date().toString() + " " + item->arrivalDateTime().time().toString("HH:mm") + " " + item->arrivalStation());
        if (!item->arrivalInfo().isEmpty()) {
            desc.append(" - " + item->arrivalInfo());
        }
        desc.append("\n");
        if (!item->info().isEmpty()) {
            desc.append(item->info() + "\n");
        }
        desc.append("--\n");
    }

    desc.append("\n(added by fahrplan app, please recheck informations before travel.)");

    #if defined(MEEGO_EDITION_HARMATTAN) || defined(Q_WS_MAEMO_5)
        QOrganizerManager defaultManager;
        QOrganizerEvent event;

        event.setDisplayLabel("Journey: " + result->departureStation() + " to " + result->arrivalStation());
        event.setDescription(desc);
        event.setStartDateTime(result->departureDateTime());
        event.setEndDateTime(result->arrivalDateTime());

        return defaultManager.saveItem(&event);
    #endif
}

void Fahrplan::onStationsResult(StationsResultList *result)
{
    emit parserStationsResult(result);
}

void Fahrplan::onJourneyResult(JourneyResultList *result)
{
    emit parserJourneyResult(result);
}

void Fahrplan::onTimeTableResult(TimeTableResultList *result)
{
    emit parserTimeTableResult(result);
}

void Fahrplan::onJourneyDetailsResult(JourneyDetailResultList *result)
{
    emit parserJourneyDetailsResult(result);
}

void Fahrplan::onErrorOccured(QString msg)
{
    emit parserErrorOccured(msg);
}
