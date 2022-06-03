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

#ifndef CALENDARSFOSWRAPPER_H
#define CALENDARSFOSWRAPPER_H

#include <QObject>
#include "parser/parser_definitions.h"

class CalendarSfosWrapper : public QObject
{
    Q_OBJECT
    public:
        explicit CalendarSfosWrapper(JourneyDetailResultList *result, QObject *parent = 0);
        virtual ~CalendarSfosWrapper();
        QString iterInfos(JourneyDetailResultItem *item);
    public slots:
        void addToCalendar();
    signals:
        void addCalendarEntryComplete(bool success);
    private:
        JourneyDetailResultList * const m_result;
};

#endif // CALENDARSFOSWRAPPER_H
