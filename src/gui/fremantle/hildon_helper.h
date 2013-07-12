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

#ifndef HILDON_HELPER_H
#define HILDON_HELPER_H

#include <QObject>
#include "qcolor.h"
#include "qfont.h"
#include "QMaemo5Style"
#include <QtMaemo5>

class HildonHelper : public QObject
{
    Q_OBJECT

    public:
        explicit HildonHelper(QObject *parent = 0);

    public slots:
        QColor standardColor(const QString &name);
        QFont standardFont(const QString &name);

};

#endif // HILDON_HELPER_H
