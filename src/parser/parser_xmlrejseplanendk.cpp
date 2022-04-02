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

#include "parser_xmlrejseplanendk.h"

ParserXmlRejseplanenDk::ParserXmlRejseplanenDk(QObject *parent) :
    ParserHafasXml(parent)
{
     baseXmlUrl = "https://www.rejseplanen.dk/bin/query.exe";
     baseSTTableUrl = "https://mobil.rejseplanen.dk/mobil-bin/stboard.exe/en";
     baseUrl = "https://www.rejseplanen.dk/bin/query.exe";

     STTableMode = 1;
}

QString ParserXmlRejseplanenDk::getTrainRestrictionsCodes(int trainrestrictions)
{
    QString trainrestr = "1111111111111111";
    if (trainrestrictions == 0) {
        trainrestr = "1111111111111111"; //ALL
    } else if (trainrestrictions == 1) {
        trainrestr = "1011111111111111"; //All without ICE
    } else if (trainrestrictions == 2) {
        trainrestr = "1111100001111111"; //All without buses
    } else if (trainrestrictions == 3) {
        trainrestr = "0000111111000000"; //Only local transport
    } else if (trainrestrictions == 4) {
        trainrestr = "0000011111000000"; //Local transport without S-Bahn
    }

    return trainrestr;
}

QStringList ParserXmlRejseplanenDk::getTrainRestrictions()
{
    QStringList result;
    result.append(tr("All"));
    result.append(tr("All without ICE"));
    result.append(tr("All without buses "));
    result.append(tr("Only local transport"));
    result.append(tr("Local transport without S-Bahn"));
    return result;
}

