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
 * Public transport Victoria (PTV) uses EFA to determine journey's, information regarding EFA can be found at the following links:
 *  main webpage from Mentz Datenverarbeitung GmbH(Munich, 02, DE), http://www.mentzdv.de/englisch/products/efa/
 *  good explaination of XML; http://data.linz.gv.at/katalog/linz_ag/linz_ag_linien/fahrplan/LINZ_LINIEN_Schnittstelle_EFA_V1.pdf
 *  documentation for the london deployment, https://code.google.com/p/openefa/
 *  http://patrickbrosi.de/?q=simpleefa#q=simpleefa
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



