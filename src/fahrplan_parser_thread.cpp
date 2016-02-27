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

#include "fahrplan_parser_thread.h"

FahrplanParserThread::FahrplanParserThread(QObject *parent) :
    QThread(parent), m_ready(false)
{
    i_parser = -1;
}

void FahrplanParserThread::init(int parserIndex)
{
    if (this->isRunning()) {
        return;
    }
    i_parser = parserIndex;
    start();
    while(!m_ready) msleep(50);
}


void FahrplanParserThread::getTimeTableForStation(const Station &currentStation, const Station &directionStation, const QDateTime &dateTime, ParserAbstract::Mode mode, int trainrestrictions)
{
    emit requestGetTimeTableForStation(currentStation, directionStation, dateTime, mode, trainrestrictions);
}

void FahrplanParserThread::findStationsByName(const QString &stationName)
{
    emit requestFindStationsByName(stationName);
}

void FahrplanParserThread::findStationsByCoordinates(qreal longitude, qreal latitude)
{
    emit requestFindStationsByCoordinates(longitude, latitude);
}

void FahrplanParserThread::searchJourney(const Station &departureStation, const Station &viaStation, const Station &arrivalStation, const QDateTime &dateTime, ParserAbstract::Mode mode, int trainrestrictions)
{
    emit requestSearchJourney(departureStation, viaStation, arrivalStation, dateTime, mode, trainrestrictions);
}

void FahrplanParserThread::searchJourneyLater()
{
    emit requestSearchJourneyLater();
}

void FahrplanParserThread::searchJourneyEarlier()
{
    emit requestSearchJourneyEarlier();
}

void FahrplanParserThread::getJourneyDetails(const QString &id)
{
    emit requestGetJourneyDetails(id);
}

void FahrplanParserThread::cancelRequest()
{
    emit requestCancelRequest();
}

QString FahrplanParserThread::name() {
    return m_name;
}

QString FahrplanParserThread::shortName() {
    return m_short_name;
}

QString FahrplanParserThread::uid() const
{
    return m_uid;
}

bool FahrplanParserThread::supportsGps()
{
    return m_supports_gps;
}
bool FahrplanParserThread::supportsVia()
{
    return m_supports_via;
}
bool FahrplanParserThread::supportsTimeTable()
{
    return m_supports_timetable;
}
bool FahrplanParserThread::supportsTimeTableDirection()
{
    return m_supports_timetabledirection;
}
QStringList FahrplanParserThread::getTrainRestrictions()
{
    return m_trainrestrictions;
}

void FahrplanParserThread::run()
{
    ParserAbstract *m_parser;

    switch (i_parser) {
        default:
        case 0:
            m_parser = new ParserMobileBahnDe();
            break;
        case 1:
            m_parser = new ParserXmlOebbAt();
            break;
        case 2:
            m_parser = new ParserXmlRejseplanenDk();
            break;
        case 3:
            m_parser = new ParserXmlSbbCh();
            break;
        case 4:
            m_parser = new ParserXmlNri();
            break;
        case 5:
            m_parser = new ParserXmlVasttrafikSe();
            break;
        case 6:
            m_parser = new ParserPTVVicGovAu();
            break;
        case 7:
            m_parser = new ParserSydneyEFA();
            break;
        case 8:
            m_parser = new ParserSFBayEFA();
            break;
        case 9:
            m_parser = new ParserIrelandEFA();
            break;
        case 10:
            m_parser = new ParserDubaiEFA();
            break;
        case 11:
            m_parser = new ParserNinetwo();
            break;
        case 12:
            m_parser = new ParserMunichEFA();
            break;
        case 13:
            m_parser = new ParserSalzburgEFA();
            break;
        case 14:
            m_parser = new ParserResRobot();
            break;
        case 15:
            m_parser = new ParserFinlandMatka();
            break;
    }

    m_name = m_parser->name();
    m_short_name = m_parser->shortName();
    m_uid = m_parser->uid();
    m_trainrestrictions = m_parser->getTrainRestrictions();
    m_supports_gps = m_parser->supportsGps();
    m_supports_via = m_parser->supportsVia();
    m_supports_timetable = m_parser->supportsTimeTable();
    m_supports_timetabledirection = m_parser->supportsTimeTableDirection();

    qRegisterMetaType<ParserAbstract::Mode>("ParserAbstract::Mode");
    //Connect thread requests with actual parser
    connect(this, SIGNAL(requestCancelRequest()), m_parser, SLOT(cancelRequest()), Qt::QueuedConnection);
    connect(this, SIGNAL(requestFindStationsByName(QString)), m_parser, SLOT(findStationsByName(QString)), Qt::QueuedConnection);
    connect(this, SIGNAL(requestFindStationsByCoordinates(qreal,qreal)), m_parser, SLOT(findStationsByCoordinates(qreal,qreal)), Qt::QueuedConnection);
    connect(this, SIGNAL(requestGetJourneyDetails(QString)), m_parser, SLOT(getJourneyDetails(QString)), Qt::QueuedConnection);
    connect(this, SIGNAL(requestGetTimeTableForStation(Station,Station,QDateTime,ParserAbstract::Mode,int)), m_parser, SLOT(getTimeTableForStation(Station,Station,QDateTime,ParserAbstract::Mode,int)), Qt::QueuedConnection);
    connect(this, SIGNAL(requestSearchJourney(Station,Station,Station,QDateTime,ParserAbstract::Mode,int)), m_parser, SLOT(searchJourney(Station,Station,Station,QDateTime,ParserAbstract::Mode,int)), Qt::QueuedConnection);
    connect(this, SIGNAL(requestSearchJourneyEarlier()), m_parser, SLOT(searchJourneyEarlier()), Qt::QueuedConnection);
    connect(this, SIGNAL(requestSearchJourneyLater()), m_parser, SLOT(searchJourneyLater()), Qt::QueuedConnection);

    //Connect parser responses with threads corresponding results
    connect(m_parser, SIGNAL(errorOccured(QString)), this, SIGNAL(errorOccured(QString)), Qt::QueuedConnection);
    connect(m_parser, SIGNAL(journeyDetailsResult(JourneyDetailResultList*)), this, SIGNAL(journeyDetailsResult(JourneyDetailResultList*)), Qt::QueuedConnection);
    connect(m_parser, SIGNAL(journeyResult(JourneyResultList*)), this, SIGNAL(journeyResult(JourneyResultList*)), Qt::QueuedConnection);
    connect(m_parser, SIGNAL(stationsResult(StationsList)), this, SIGNAL(stationsResult(StationsList)), Qt::QueuedConnection);
    connect(m_parser, SIGNAL(timetableResult(TimetableEntriesList)), this, SIGNAL(timeTableResult(TimetableEntriesList)), Qt::QueuedConnection);

    m_ready = true;

    // Autodelete after thread finishes.
    connect(this, SIGNAL(finished()), SLOT(deleteLater()));

    exec();

    delete m_parser;
}
