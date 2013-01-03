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

#ifndef PARSER_XMLOEBBAT_H
#define PARSER_XMLOEBBAT_H

#include "parser_hafasxml.h"

class ParserXmlOebbAt : public ParserHafasXml
{
    Q_OBJECT

public:
    explicit ParserXmlOebbAt(QObject *parent = 0);
    static QString getName() { return "öbb.at"; }
    QString name() { return "öbb.at"; }

public slots:
    QStringList getTrainRestrictions();
    bool supportsTimeTable();
};

#endif // PARSER_XMLOEBBAT_H
