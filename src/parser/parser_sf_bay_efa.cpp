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

/* Overview
 * This parser was written by Evan Sebire
 *
 */


#include "parser_sf_bay_efa.h"

#include <QBuffer>
#include <QDomDocument>
#include <QFile>
#include <QNetworkReply>


ParserSFBayEFA::ParserSFBayEFA(QObject *parent) :
    ParserEFA(parent)
{
    baseRestUrl = "http://tripplanner.transit.511.org/mtc/";
}


QStringList ParserSFBayEFA::getTrainRestrictions()
{
    QStringList result;
    result.append(tr("All"));
    result.append(tr("AC Transit (Local Service)"));
    result.append(tr("AC Transit (Transbay Service)"));
    result.append(tr("ACE"));
    result.append(tr("AirBART"));
    result.append(tr("American Canyon Transit"));
    result.append(tr("Amtrak Capitol Cor. & Reg. Svc"));
    result.append(tr("Angel Island - Tiburon Ferry"));
    result.append(tr("BART"));
    result.append(tr("Blue & Gold Fleet"));
    result.append(tr("Caltrain"));
    result.append(tr("County Connection"));
    result.append(tr("Dumbarton Express"));
    result.append(tr("Emery Go-Round"));
    result.append(tr("Fairfield and Suisun Transit (FAST)"));
    result.append(tr("Golden Gate Ferry"));
    result.append(tr("Golden Gate Transit"));
    result.append(tr("Hornblower Alcatraz Ferry"));
    result.append(tr("Marin Transit"));
    result.append(tr("Petaluma Transit"));
    result.append(tr("Rio Vista Delta Breeze"));
    result.append(tr("SamTrans"));
    result.append(tr("San Francisco Muni"));
    result.append(tr("Santa Clara VTA"));
    result.append(tr("Santa Rosa CityBus"));
    result.append(tr("SF Bay Ferry"));
    result.append(tr("SolTrans"));
    result.append(tr("Sonoma County Transit"));
    result.append(tr("St Helena VINE"));
    result.append(tr("Stanford Marguerite Shuttle"));
    result.append(tr("Tri Delta Transit"));
    result.append(tr("Union City Transit"));
    result.append(tr("Vacaville City Transit"));
    result.append(tr("VINE (Napa County)"));
    result.append(tr("WestCAT"));
    result.append(tr("WHEELS"));
    result.append(tr("Yountville Shuttle"));
    return result;
}



