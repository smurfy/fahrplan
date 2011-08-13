/*******************************************************************************

    This file is a part of Fahrplan for maemo 2009-2010

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

#include "parser_abstract.h"

void ParserAbstract::networkReplyFinished(QNetworkReply *networkReply)
{
    if (currentRequestState == Fahrplan::stationsByNameRequest) {
        parseStationsByName(networkReply);
    } else if (currentRequestState == Fahrplan::stationsByCoordinatesRequest) {
        parseStationsByCoordinates(networkReply);
    } else {
        qDebug()<<"Current request unhandled!";
    }
    currentRequestState = Fahrplan::noneRequest;
}

bool ParserAbstract::supportsGps()
{
    return false;
}

void ParserAbstract::findStationsByName(QString stationName)
{
    Q_UNUSED(stationName);
}

void ParserAbstract::findStationsByCoordinates(qreal longitude, qreal latitude)
{
    Q_UNUSED(longitude);
    Q_UNUSED(latitude);
}

 void ParserAbstract::parseStationsByName(QNetworkReply *networkReply)
 {
    Q_UNUSED(networkReply);
    qDebug() << "ParserAbstract::parseStationsByName";
 }

 void ParserAbstract::parseStationsByCoordinates(QNetworkReply *networkReply)
 {
     Q_UNUSED(networkReply);
     qDebug() << "ParserAbstract::parseStationsByCoordinates";
 }
