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

#include "parser_xmloebbat.h"

ParserXmlOebbAt::ParserXmlOebbAt(QObject *parent)
    : ParserHafasBinary(parent)
{
    baseXmlUrl = "http://fahrplan.oebb.at/bin/query.exe";
    baseSTTableUrl = "http://fahrplan.oebb.at/bin/stboard.exe/en";
    baseUrl = "http://fahrplan.oebb.at/bin/query.exe";
    baseBinaryUrl = "http://fahrplan.oebb.at/bin/query.exe/en";
    STTableMode = 1;
}

QStringList ParserXmlOebbAt::getTrainRestrictions()
{
    QStringList result;
    result.append(tr("All"));
    result.append(tr("All without ICE, TGV"));
    result.append(tr("Einfach-Raus-Ticket"));
    return result;
}

QString ParserXmlOebbAt::getTrainRestrictionsCodes(int trainrestrictions)
{
    QString trainrestr = "1111111111010000";
    if (trainrestrictions == 0) {
        trainrestr = "1111111111010000"; //ALL
    } else if (trainrestrictions == 1) {
        trainrestr = "0111111111010000"; //All without ICE, TGV
    } else if (trainrestrictions == 2) {
        trainrestr = "0000110011000000"; //Einfach-Raus-Ticket
    }
    return trainrestr;
}
