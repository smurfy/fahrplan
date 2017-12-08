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

#include "parser_mobilebahnde.h"

ParserMobileBahnDe::ParserMobileBahnDe(QObject *parent) :
    ParserHafasBinary(parent)
{
     baseXmlUrl = "https://reiseauskunft.bahn.de/bin/query.exe";
     baseSTTableUrl = "https://mobile.bahn.de/bin/mobil/bhftafel.exe/en";
     baseUrl = "https://reiseauskunft.bahn.de/bin/query.exe";
     baseBinaryUrl = "https://reiseauskunft.bahn.de/bin/query.exe/eox";
     STTableMode = 1;
}

QStringList ParserMobileBahnDe::getTrainRestrictions()
{
    QStringList result;
    result.append(tr("All"));
    result.append(tr("All without ICE"));
    result.append(tr("Only local transport"));
    result.append(tr("Local transport without S-Bahn"));
    return result;
}

QString ParserMobileBahnDe::getTrainRestrictionsCodes(int trainrestrictions)
{
    QString trainrestr = "1:1111111111000000";
    if (trainrestrictions == 0) {
        trainrestr = "1:1111111111000000"; //ALL
    } else if (trainrestrictions == 1) {
        trainrestr = "2:0111111111000000"; //All without ICE
    } else if (trainrestrictions == 2) {
        trainrestr = "4:00011111110000000"; //Only local transport
    } else if (trainrestrictions == 3) {
        trainrestr = "4:0001011111000000"; //Only local transport without S-Bahn
    }

    return trainrestr;
}

