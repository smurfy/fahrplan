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

#ifndef PARSER_XMLREJSEPLANENDK_H
#define PARSER_XMLREJSEPLANENDK_H

#include "parser_hafasxml.h"

class ParserXmlRejseplanenDk : public ParserHafasXml
{
    Q_OBJECT

public:
    explicit ParserXmlRejseplanenDk(QObject *parent = 0);
    static QString getName() { return "rejseplanen.dk"; }
    QString name() { return "rejseplanen.dk"; }

protected:
    QStringList getTrainRestrictions();
    QString getTrainRestrictionsCodes(int trainrestrictions);
};

#endif // PARSER_XMLREJSEPLANENDK_H
