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

#ifndef CALENDARTHREADWRAPPER_H
#define CALENDARTHREADWRAPPER_H

#include <QObject>
#include "parser/parser_definitions.h"
#include <QThread>
#include <QCoreApplication>

#if defined(MEEGO_EDITION_HARMATTAN) || defined(Q_WS_MAEMO_5) || defined(Q_OS_SYMBIAN)

#include <qmobilityglobal.h>
#include <QOrganizerManager>
#include <QOrganizerEvent>

QTM_BEGIN_NAMESPACE
QTM_END_NAMESPACE
QTM_USE_NAMESPACE

#endif

class CalendarThreadWrapper : public QObject
{
    Q_OBJECT
    public:
        explicit CalendarThreadWrapper(JourneyDetailResultList *result, QObject *parent = 0);
        virtual ~CalendarThreadWrapper();
    public slots:
        void addToCalendar();
    signals:
        void addCalendarEntryComplete(bool success);
    private:
        JourneyDetailResultList * const m_result;
};

#endif // CALENDARTHREADWRAPPER_H
