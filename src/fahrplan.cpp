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

FahrplanBackendManager::FahrplanBackendManager(QObject *parent) :
    QObject(parent)
{
}

QStringList FahrplanBackendManager::getParserList()
{
    QStringList result;
    result.append(ParserXmlOebbAt::getName());
    result.append(ParserXmlRejseplanenDk::getName());
    result.append(ParserXmlSbbCh::getName());
    result.append(ParserHafasXml::getName());
    return result;
}

ParserAbstract *FahrplanBackendManager::getParser()
{
    if (!m_parser) {
        setParser(0);
    }
    return m_parser;
}

void FahrplanBackendManager::setParser(int index)
{
    if (currentParserIndex == index && m_parser) {
        return;
    }

    currentParserIndex = index;

    /*
    if (m_parser) {
        delete m_parser;
    }
    */

    switch (index) {
        case 0:
            m_parser = new ParserXmlOebbAt();
            break;
        case 1:
            m_parser = new ParserXmlRejseplanenDk();
            break;
        case 2:
            m_parser = new ParserXmlSbbCh();
            break;
        case 3:
            m_parser = new ParserHafasXml();
            break;
    }

    emit parserChanged(m_parser->name());
}

Fahrplan::Fahrplan(QObject *parent) :
    QObject(parent)
{
    if (!m_parser_manager) {
        m_parser_manager = new FahrplanBackendManager();
    }

    connect(m_parser_manager, SIGNAL(parserChanged(QString)), this, SLOT(onParserChanged(QString)));
}

ParserAbstract* Fahrplan::parser()
{
    return m_parser_manager->getParser();
}

void Fahrplan::onParserChanged(QString name)
{
    //We need to reconnect all Signals to the new Parser
    connect(m_parser_manager->getParser(), SIGNAL(stationsResult(StationsResultList*)), this, SLOT(onStationsResult(StationsResultList*)));
    connect(m_parser_manager->getParser(), SIGNAL(journeyResult(JourneyResultList*)), this, SLOT(onJourneyResult(JourneyResultList*)));
    connect(m_parser_manager->getParser(), SIGNAL(errorOccured(QString)), this, SLOT(onErrorOccured(QString)));
    connect(m_parser_manager->getParser(), SIGNAL(journeyDetailsResult(JourneyDetailResultList*)), this, SLOT(onJourneyDetailsResult(JourneyDetailResultList*)));

    emit parserChanged(name);
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
    m_parser_manager->setParser(index);
}

void Fahrplan::onStationsResult(StationsResultList *result)
{
    emit parserStationsResult(result);
}

void Fahrplan::onJourneyResult(JourneyResultList *result)
{
    emit parserJourneyResult(result);
}

void Fahrplan::onJourneyDetailsResult(JourneyDetailResultList *result)
{
    emit parserJourneyDetailsResult(result);
}

void Fahrplan::onErrorOccured(QString msg)
{
    emit parserErrorOccured(msg);
}
