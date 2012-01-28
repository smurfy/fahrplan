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

#include "parser_xmlsbbch.h"

ParserXmlSbbCh::ParserXmlSbbCh(QObject *parent)
{
     Q_UNUSED(parent);
     baseXmlUrl = "http://fahrplan.sbb.ch/bin/extxml.exe";
     baseUrl = "http://fahrplan.sbb.ch/bin/query.exe";

     hafasHeader.accessid = "MJXZ841ZfsmqqmSymWhBPy5dMNoqoGsHInHbWJQ5PTUZOJ1rLTkn8vVZOZDFfSe";
     hafasHeader.prod = "iPhone3.1";
     hafasHeader.ver = "2.3";
}

bool ParserXmlSbbCh::supportsTimeTable()
{
    return true;
}

void ParserXmlSbbCh::parseStationsByName(QNetworkReply *networkReply)
{
    QString data = QString::fromUtf8(networkReply->readAll());
    StationsResultList *result = internalParseStationsByName(data);
    emit stationsResult(result);
}

QString ParserXmlSbbCh::getTrainRestrictionsCodes(int trainrestrictions)
{
    QString trainrestr = "1111111111111111";
    if (trainrestrictions == 0) {
        trainrestr = "1111111111111111"; //ALL
    } else if (trainrestrictions == 1) {
        trainrestr = "0111111111000000"; //All without ICE
    }

    return trainrestr;
}

QStringList ParserXmlSbbCh::getTrainRestrictions()
{
    QStringList result;
    result.append(tr("All"));
    result.append(tr("All without ICE"));
    return result;
}
