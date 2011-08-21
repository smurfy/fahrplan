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

#ifndef FAHRPLAN_H
#define FAHRPLAN_H

#include <QObject>
#include "parser/parser_hafasxml.h"
#include "parser/parser_xmloebbat.h"
#include "parser/parser_xmlrejseplanendk.h"
#include "parser/parser_xmlsbbch.h"

class Fahrplan : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ParserAbstract *parser READ parser)
    Q_PROPERTY(QString parserName READ parserName)

    public:
        explicit Fahrplan(QObject *parent = 0);
        ParserAbstract* parser();
        QString parserName();

    public slots:
        QStringList getParserList();
        void setParser(int index);

    signals:
        void parserStationsResult(StationsResultList *result);
        void parserJourneyResult(JourneyResultList *result);
        void parserJourneyDetailsResult(JourneyDetailResultList *result);
        void parserErrorOccured(QString msg);
        void parserChanged(QString name);

    private slots:
        void stationsResult(StationsResultList *result);
        void journeyResult(JourneyResultList *result);
        void journeyDetailsResult(JourneyDetailResultList *result);
        void errorOccured(QString msg);
        void parserDestroyed(QObject *obj);

    private:
        static ParserAbstract *m_parser;
        static int currentParserIndex;
        bool reconnectSignals;
};

#endif // FAHRPLAN_H
