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

#ifndef PARSER_XMLRMVDE_H
#define PARSER_XMLRMVDE_H

#include "parser_hafasxml.h"

class ParserXmlRMVde : public ParserHafasXml
{
    Q_OBJECT

public:
    explicit ParserXmlRMVde(QObject *parent = 0);
    static QString getName() { return QString(QLatin1String("%1 (rmv.de)")).arg(tr("Germany, Hesse")); }
    virtual QString name() { return getName(); }
    virtual QString shortName() { return QLatin1String("rmv.de"); }

protected:
    void parseStationsByName(QNetworkReply *networkReply);
    QStringList getTrainRestrictions();
    QString getTrainRestrictionsCodes(int trainrestrictions);
};

#endif // PARSER_XMLRMVDE_H
