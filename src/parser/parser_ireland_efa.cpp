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

/*
 * This parser was written by Evan Sebire
 *  Irish national journey planner including northern Ireland
 *  server uses EFAv10
 */


#include "parser_ireland_efa.h"

#include <QBuffer>
#include <QDomDocument>
#include <QFile>
#include <QNetworkReply>


ParserIrelandEFA::ParserIrelandEFA(QObject *parent) :
    ParserEFA(parent)
{
    baseRestUrl = "https://www.journeyplanner.transportforireland.ie/nta/";
}


QStringList ParserIrelandEFA::getTrainRestrictions()
{
    QStringList result;
    result.append(tr("All"));
    result.append(tr("Train"));
    result.append(tr("Tram"));
    result.append(tr("Bus"));
    result.append(tr("Ferry"));
    result.append(tr("Taxi"));
    return result;
}



