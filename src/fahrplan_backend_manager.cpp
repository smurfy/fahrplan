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

#include "fahrplan_backend_manager.h"

FahrplanBackendManager::FahrplanBackendManager(int defaultParser, QObject *parent) :
    QObject(parent)
{
    m_parser = NULL;
    currentParserIndex = defaultParser;
}

QStringList FahrplanBackendManager::getParserList()
{
    QStringList result;
    result.append(ParserMobileBahnDe::getName());
    result.append(ParserXmlOebbAt::getName());
    result.append(ParserXmlRejseplanenDk::getName());
    result.append(ParserXmlSbbCh::getName());
    result.append(ParserXmlNri::getName());
    result.append(ParserXmlVasttrafikSe::getName());
    result.append(ParserPTVVicGovAu::getName());
    result.append(ParserSydneyEFA::getName());
    result.append(ParserSFBayEFA::getName());
    result.append(ParserIrelandEFA::getName());
    result.append(ParserDubaiEFA::getName());
    result.append(ParserNinetwo::getName());
    result.append(ParserMunichEFA::getName());
    result.append(ParserSalzburgEFA::getName());
    result.append(ParserResRobot::getName());
    result.append(ParserFinlandMatka::getName());

    // Make sure the index is in bounds
    if (currentParserIndex > (result.count() - 1) || currentParserIndex < 0) {
        currentParserIndex = 0;
    }

    return result;
}

FahrplanParserThread *FahrplanBackendManager::getParser()
{
    if (!m_parser) {
        setParser(currentParserIndex);
    }
    return m_parser;
}

void FahrplanBackendManager::setParser(int index)
{
    if (currentParserIndex == index && m_parser) {
        return;
    }

    if (m_parser) {
        // Parser object will be autodeleted after the thread quits.
        m_parser->quit();
    }

    m_parser = new FahrplanParserThread();
    m_parser->init(index);

    // Init can fallback to another index if its invalid.
    currentParserIndex = m_parser->getParserIndex();

    emit parserChanged(m_parser->name(), currentParserIndex);
}
