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

#include "calendarthreadwrapper.h"

#include <QThread>

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

    connect(m_parser_manager, SIGNAL(parserChanged(const QString &, int)), this, SLOT(onParserChanged(const QString &, int)));
    connect(m_favorites_manager, SIGNAL(favoritesChanged(const QStringList &)), this, SIGNAL(favoritesChanged(QStringList)));
}

void Fahrplan::bindParserSignals()
{
    if (m_parser_manager->getParser()) {
        connect(m_parser_manager->getParser(), SIGNAL(stationsResult(StationsResultList*)), this, SIGNAL(parserStationsResult(StationsResultList*)));
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

FahrplanFavoritesManager* Fahrplan::favorites()
{
    return m_favorites_manager;
}

QString Fahrplan::getVersion()
{
    return FAHRPLAN_VERSION;
}

void Fahrplan::onParserChanged(const QString &name, int index)
{
    //We need to reconnect all Signals to the new Parser
    bindParserSignals();
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

