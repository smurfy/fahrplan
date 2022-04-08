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

#include "parser_xmlnri.h"

ParserXmlNri::ParserXmlNri(QObject *parent) :
    ParserHafasXml(parent)
{
    // new service at https://www.vy.no/se-reiseforslag
    baseXmlUrl = "https://hafas.websrv05.reiseinfo.no/bin/dev/nri/query.exe";
    baseSTTableUrl = "https://hafas.websrv05.reiseinfo.no/bin/dev/nri/stboard.exe/en";
    baseUrl = "https://hafas.websrv05.reiseinfo.no/bin/dev/nri/query.exe";
    STTableMode = 1;
}

QStringList ParserXmlNri::getTrainRestrictions()
{
    QStringList result;
    result.append(tr("All"));
    return result;
}

QString ParserXmlNri::getTrainRestrictionsCodes(int trainrestrictions)
{
    Q_UNUSED(trainrestrictions)
    QString trainrestr = "1111111111111111";
    return trainrestr;
}
