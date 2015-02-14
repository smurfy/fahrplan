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

#include "parser_munich_efa.h"

#include <QBuffer>
#include <QDomDocument>
#include <QFile>
#include <QNetworkReply>

ParserMunichEFA::ParserMunichEFA(QObject *parent) :
    ParserEFA(parent)
{
    //baseRestUrl = "http://efa-alt.mvv-muenchen.de/mvv/";
    baseRestUrl = "http://efa.mvv-muenchen.de/mobile/";
    acceptEncoding = "gzip";
}

QStringList ParserMunichEFA::getTrainRestrictions()
{
    QStringList result;
    result.append(tr("All"));
    result.append(tr("S-Bahn"));
    result.append(tr("U-Bahn"));
    result.append(tr("Tram"));
    result.append(tr("Bus"));
    return result;
}
