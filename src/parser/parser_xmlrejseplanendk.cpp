/*******************************************************************************

    This file is a part of Fahrplan for maemo 2009-2010

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

#include "parser_xmlrejseplanendk.h"

ParserXmlRejseplanenDk::ParserXmlRejseplanenDk(QObject *parent)
{
     baseUrl = "http://www.rejseplanen.dk/bin/query.exe";
}

QString ParserXmlRejseplanenDk::getTrainRestrictionsCodes(int trainrestrictions)
{
    QString trainrestr = "1111111111111111";
    if (trainrestrictions == 0) {
        trainrestr = "1111111111111111"; //ALL
    } else if (trainrestrictions == 1) {
        trainrestr = "0111111111000000"; //All without ICE
    }

    return trainrestr;
}

QStringList ParserXmlRejseplanenDk::getTrainRestrictions()
{
    QStringList result;
    result.append(tr("All"));
    result.append(tr("All without ICE"));
    return result;
}

