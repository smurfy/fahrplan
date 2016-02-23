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

#ifndef FAHRPLAN_PARSER_THREAD_H
#define FAHRPLAN_PARSER_THREAD_H

#include <QThread>
#include "parser/parser_abstract.h"
#include "parser/parser_hafasxml.h"
#include "parser/parser_hafasbinary.h"
#include "parser/parser_xmloebbat.h"
#include "parser/parser_xmlvasttrafikse.h"
#include "parser/parser_xmlrejseplanendk.h"
#include "parser/parser_xmlsbbch.h"
#include "parser/parser_xmlnri.h"
#include "parser/parser_mobilebahnde.h"
#include "parser/parser_ptvvicgovau.h"
#include "parser/parser_sf_bay_efa.h"
#include "parser/parser_sydney_efa.h"
#include "parser/parser_ireland_efa.h"
#include "parser/parser_dubai_efa.h"
#include "parser/parser_ninetwo.h"
#include "parser/parser_munich_efa.h"
#include "parser/parser_salzburg_efa.h"
#include "parser/parser_resrobot.h"
#include "parser/parser_finland_matka.h"

class FahrplanParserThread : public QThread
{
    Q_OBJECT
public:
    explicit FahrplanParserThread(QObject *parent = 0);

signals:
    //Internal
    void requestGetTimeTableForStation(const Station &stationName, const Station &directionStationName, const QDateTime &dateTime, ParserAbstract::Mode mode, int trainrestrictions);
    void requestFindStationsByName(const QString &stationName);
    void requestFindStationsByCoordinates(qreal longitude, qreal latitude);
    void requestSearchJourney(const Station &departureStation, const Station &viaStation, const Station &arrivalStation, const QDateTime &dateTime, ParserAbstract::Mode mode, int trainrestrictions);
    void requestSearchJourneyLater();
    void requestSearchJourneyEarlier();
    void requestGetJourneyDetails(const QString &id);
    void requestCancelRequest();

    //Real ones
    void stationsResult(const StationsList &result);
    void journeyResult(JourneyResultList *result);
    void journeyDetailsResult(JourneyDetailResultList *result);
    void timeTableResult(const TimetableEntriesList &result);
    void errorOccured(QString msg);

public slots:
    void init(int parserIndex);

    void getTimeTableForStation(const Station &currentStation, const Station &directionStation, const QDateTime &dateTime, ParserAbstract::Mode mode, int trainrestrictions);
    void findStationsByName(const QString &stationName);
    void findStationsByCoordinates(qreal longitude, qreal latitude);
    void searchJourney(const Station &departureStation, const Station &viaStation, const Station &arrivalStation, const QDateTime &dateTime, ParserAbstract::Mode mode, int trainrestrictions);
    void searchJourneyLater();
    void searchJourneyEarlier();
    void getJourneyDetails(const QString &id);
    void cancelRequest();

    bool supportsGps();
    bool supportsVia();
    bool supportsTimeTable();
    bool supportsTimeTableDirection();
    QString name();
    QString shortName();
    QString uid() const;
    QStringList getTrainRestrictions();

protected:
  void run();

private:
  bool m_ready;
  int  i_parser;

  QStringList m_trainrestrictions;
  bool m_supports_gps;
  bool m_supports_via;
  bool m_supports_timetable;
  bool m_supports_timetabledirection;
  QString m_name;
  QString m_short_name;
  QString m_uid;
};

#endif // FAHRPLAN_PARSER_THREAD_H
