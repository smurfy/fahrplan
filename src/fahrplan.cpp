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

ParserAbstract *Fahrplan::m_parser;
int Fahrplan::currentParserIndex;

Fahrplan::Fahrplan(QObject *parent) :
    QObject(parent)
{
    reconnectSignals = true;
}

ParserAbstract* Fahrplan::parser()
{
    if (!m_parser) {
        setParser(0);
    }

    if (reconnectSignals) {
        //We need to reconnect all Signals to the new Parser
        connect(m_parser, SIGNAL(stationsResult(StationsResultList*)), this, SLOT(stationsResult(StationsResultList*)));
        connect(m_parser, SIGNAL(journeyResult(JourneyResultList*)), this, SLOT(journeyResult(JourneyResultList*)));
        connect(m_parser, SIGNAL(errorOccured(QString)), this, SLOT(errorOccured(QString)));
        connect(m_parser, SIGNAL(journeyDetailsResult(JourneyDetailResultList*)), this, SLOT(journeyDetailsResult(JourneyDetailResultList*)));
        connect(m_parser, SIGNAL(destroyed(QObject*)), this, SLOT(parserDestroyed(QObject*)));
    }

    return m_parser;
}

QString Fahrplan::parserName()
{
    return parser()->name();
}

QStringList Fahrplan::getParserList()
{
    QStringList result;
    result.append(ParserXmlOebbAt::getName());
    result.append(ParserXmlRejseplanenDk::getName());
    result.append(ParserXmlSbbCh::getName());
    result.append(ParserHafasXml::getName());
    return result;
}

void Fahrplan::setParser(int index)
{
    if (currentParserIndex == index && m_parser) {
        return;
    }

    currentParserIndex = index;

    if (m_parser) {
        delete m_parser;
    }

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

    emit parserChanged(parserName());
}

void Fahrplan::parserDestroyed(QObject *obj)
{
    reconnectSignals = true;
}

void Fahrplan::stationsResult(StationsResultList *result)
{
    emit parserStationsResult(result);
}

void Fahrplan::journeyResult(JourneyResultList *result)
{
    emit parserJourneyResult(result);
}

void Fahrplan::journeyDetailsResult(JourneyDetailResultList *result)
{
    emit parserJourneyDetailsResult(result);
}

void Fahrplan::errorOccured(QString msg)
{
    emit parserErrorOccured(msg);
}
