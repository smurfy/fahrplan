/*******************************************************************************

    This file is a part of Fahrplan for maemo 2009-2011

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

#include "calendarthreadwrapper.h"

CalendarThreadWrapper::CalendarThreadWrapper(JourneyDetailResultList *result, QObject *parent) :
    QObject(parent), m_result(result)
{
}

CalendarThreadWrapper::~CalendarThreadWrapper()
{
    qDebug() << "Destroyed";
}

void CalendarThreadWrapper::addToCalendar()
{
    #if defined(MEEGO_EDITION_HARMATTAN) || defined(Q_WS_MAEMO_5)

    QString desc;
    desc.append(m_result->departureStation() + tr(" to ") + m_result->arrivalStation() + "\n");

    if (!m_result->info().isEmpty()) {
        desc.append(m_result->info() + "\n");
    }

    for (int i=0; i < m_result->itemcount(); i++) {
        JourneyDetailResultItem *item = m_result->getItem(i);

        if (!item->train().isEmpty()) {
            desc.append(item->train() + "\n");
        }
        desc.append(item->departureDateTime().date().toString() + " " + item->departureDateTime().time().toString("HH:mm") + " " + item->departureStation());
        if (!item->departureInfo().isEmpty()) {
            desc.append(" - " + item->departureInfo());
        }
        desc.append("\n");
        desc.append(item->arrivalDateTime().date().toString() + " " + item->arrivalDateTime().time().toString("HH:mm") + " " + item->arrivalStation());
        if (!item->arrivalInfo().isEmpty()) {
            desc.append(" - " + item->arrivalInfo());
        }
        desc.append("\n");
        if (!item->info().isEmpty()) {
            desc.append(item->info() + "\n");
        }
        desc.append("--\n");
    }

    desc.append(tr("\n(added by fahrplan app, please recheck informations before travel.)"));


    QOrganizerManager defaultManager;
    QOrganizerEvent event;

    event.setDisplayLabel(tr("Journey: %1 to %2").arg(m_result->departureStation()).arg(m_result->arrivalStation()));
    event.setDescription(desc);
    event.setStartDateTime(m_result->departureDateTime());
    event.setEndDateTime(m_result->arrivalDateTime());

    emit addCalendarEntryComplete(defaultManager.saveItem(&event));

    #else

    emit addCalendarEntryComplete(false);

    #endif

    QThread::currentThread()->exit(0);

    // Move back to GUI thread so the deleteLater() callback works (it requires
    // an event loop which is still alive)
    moveToThread(QCoreApplication::instance()->thread());
}
