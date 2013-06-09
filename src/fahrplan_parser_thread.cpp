/*******************************************************************************

    This file is a part of Fahrplan for maemo 2009-2012

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


void FahrplanParserThread::getTimeTableForStation(const QString &stationName, const QString &directionStationName, const QDate &date, const QTime &time, ParserAbstract::Mode mode, int trainrestrictions)
{
    emit requestGetTimeTableForStation(stationName, directionStationName, date, time, mode, trainrestrictions);
}

void FahrplanParserThread::findStationsByName(const QString &stationName)
{
    emit requestFindStationsByName(stationName);
}

void FahrplanParserThread::findStationsByCoordinates(qreal longitude, qreal latitude)
{
    emit requestFindStationsByCoordinates(longitude, latitude);
}

void FahrplanParserThread::searchJourney(const QString &departureStation, const QString &arrivalStation, const QString &viaStation, const QDate &date, const QTime &time, ParserAbstract::Mode mode, int trainrestrictions)
{
    emit requestSearchJourney(departureStation, arrivalStation, viaStation, date, time, mode, trainrestrictions);
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
          m_parser = new Parser131500ComAu();
          break;
      case 5:
          m_parser = new ParserXmlNri();
          break;
      case 6:
          m_parser = new ParserXmlVasttrafikSe();
          break;
    }

    m_name = m_parser->name();
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
    connect(this, SIGNAL(requestGetTimeTableForStation(QString,QString,QDate,QTime,ParserAbstract::Mode,int)), m_parser, SLOT(getTimeTableForStation(QString,QString,QDate,QTime,ParserAbstract::Mode,int)), Qt::QueuedConnection);
    connect(this, SIGNAL(requestSearchJourney(QString,QString,QString,QDate,QTime,ParserAbstract::Mode,int)), m_parser, SLOT(searchJourney(QString,QString,QString,QDate,QTime,ParserAbstract::Mode,int)), Qt::QueuedConnection);
    connect(this, SIGNAL(requestSearchJourneyEarlier()), m_parser, SLOT(searchJourneyEarlier()), Qt::QueuedConnection);
    connect(this, SIGNAL(requestSearchJourneyLater()), m_parser, SLOT(searchJourneyLater()), Qt::QueuedConnection);

    //Connect parser responses with threads corresponding results
    connect(m_parser, SIGNAL(errorOccured(QString)), this, SIGNAL(errorOccured(QString)), Qt::QueuedConnection);
    connect(m_parser, SIGNAL(journeyDetailsResult(JourneyDetailResultList*)), this, SIGNAL(journeyDetailsResult(JourneyDetailResultList*)), Qt::QueuedConnection);
    connect(m_parser, SIGNAL(journeyResult(JourneyResultList*)), this, SIGNAL(journeyResult(JourneyResultList*)), Qt::QueuedConnection);
    connect(m_parser, SIGNAL(stationsResult(StationsResultList*)), this, SIGNAL(stationsResult(StationsResultList*)), Qt::QueuedConnection);
    connect(m_parser, SIGNAL(timeTableResult(TimeTableResultList*)), this, SIGNAL(timeTableResult(TimeTableResultList*)), Qt::QueuedConnection);

    m_ready = true;
    exec();
}
