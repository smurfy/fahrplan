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

#ifndef PARSER_DUBAI_EFA_H
#define PARSER_DUBAI_EFA_H

#include <QObject>
#include <QXmlResultItems>
#include "parser_efa.h"

class ParserDubaiEFA : public ParserEFA
{
    Q_OBJECT
public:
    explicit ParserDubaiEFA(QObject *parent = 0);
    static QString getName() { return "Dubai"; }
    QString name() { return "Dubai"; }

protected:
    QStringList getTrainRestrictions();

};


#endif // PARSER_DUBAI_EFA_H
