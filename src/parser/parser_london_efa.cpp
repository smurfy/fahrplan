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
 *  Server uses EFAv10
 */


#include "parser_london_efa.h"

#include <QBuffer>
#include <QDomDocument>
#include <QFile>
#include <QNetworkReply>


ParserLondonEFA::ParserLondonEFA(QObject *parent) :
    ParserEFA(parent)
{
    baseRestUrl = "http://journeyplanner.tfl.gov.uk/user/";
}


QStringList ParserLondonEFA::getTrainRestrictions()
{
    QStringList result;
    result.append(tr("All"));
    result.append(tr("Tube"));
    result.append(tr("Rail"));
    result.append(tr("Coach"));
    result.append(tr("DLR"));
    result.append(tr("Tram"));
    result.append(tr("Bus"));
    result.append(tr("River"));
    result.append(tr("Emirates Air Line"));
    result.append(tr("London Overground"));
    return result;
}



