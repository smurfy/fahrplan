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

#ifndef PARSER_VRR_EFA_H
#define PARSER_VRR_EFA_H

#include <QObject>
#include "parser_efa.h"

class ParserVRREFA : public ParserEFA
{
    Q_OBJECT
public:
    explicit ParserVRREFA(QObject *parent = 0);
    static QString getName() { return QString("%1, %2 (vrr.de)").arg(tr("Germany"), tr("Rhine Ruhr")); }
    virtual QString name() { return getName(); }
    virtual QString shortName() { return "vrr.de"; }

protected:
    QStringList getTrainRestrictions();

};


#endif // PARSER_VRR_EFA_H
