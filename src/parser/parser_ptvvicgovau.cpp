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

/* Overview
 * This parser was written by Evan Sebire
 * Public transport Victoria (PTV) uses EFAv9 to determine journey's.
 *
 */


#include "parser_ptvvicgovau.h"

#include <QBuffer>
#include <QDomDocument>
#include <QFile>
#include <QNetworkReply>


ParserPTVVicGovAu::ParserPTVVicGovAu(QObject *parent) :
    ParserEFA(parent)
{
    baseRestUrl = "http://jp.ptv.vic.gov.au/ptv/";

}


QStringList ParserPTVVicGovAu::getTrainRestrictions()
{
    QStringList result;
    result.append(tr("All"));
    result.append(tr("Metro Train"));
    result.append(tr("Metro Tram"));
    result.append(tr("Metro Bus"));
    result.append(tr("Regional Train"));
    result.append(tr("Regional Coach"));
    result.append(tr("Airport(SKYBUS)"));
    return result;
}



