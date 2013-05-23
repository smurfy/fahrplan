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

#include "calendarthreadwrapper.h"

#include <QCoreApplication>
#include <QThread>

#ifdef Q_OS_BLACKBERRY
#   include <bb/pim/calendar/CalendarService>
#   include <bb/pim/calendar/CalendarEvent>
using namespace bb::pim::calendar;
#elif defined(MEEGO_EDITION_HARMATTAN) || defined(Q_WS_MAEMO_5) || defined(Q_OS_SYMBIAN)
#   include <qmobilityglobal.h>
#   include <QOrganizerManager>
#   include <QOrganizerEvent>
QTM_USE_NAMESPACE
#endif

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
    QString desc;
    const QString viaStation = m_result->viaStation();
    if (viaStation.isEmpty())
        desc.append(tr("%1 to %2").arg(m_result->departureStation()).arg(m_result->arrivalStation()) + "\n");
    else
        desc.append(tr("%1 via %3 to %2").arg(m_result->departureStation()).arg(m_result->arrivalStation()).arg(viaStation) + "\n");

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

#ifdef Q_OS_BLACKBERRY

    CalendarService service;
    QPair<AccountId, FolderId> folder = service.defaultCalendarFolder();

    CalendarEvent event;
    event.setAccountId(folder.first);
    event.setFolderId(folder.second);
    if (viaStation.isEmpty())
        event.setSubject(tr("Journey: %1 to %2").arg(m_result->departureStation()).arg(m_result->arrivalStation()));
    else
        event.setSubject(tr("Journey: %1 via %3 to %2").arg(m_result->departureStation()).arg(m_result->arrivalStation()).arg(viaStation));
    event.setStartTime(m_result->departureDateTime());
    event.setEndTime(m_result->arrivalDateTime());
    event.setBody(desc);
    event.setReminder(-1);

    emit addCalendarEntryComplete(service.createEvent(event) == Result::Success);

#elif defined(MEEGO_EDITION_HARMATTAN) || defined(Q_WS_MAEMO_5) || defined(Q_OS_SYMBIAN)

    QOrganizerManager defaultManager;
    QOrganizerEvent event;

    if (viaStation.isEmpty())
        event.setDisplayLabel(tr("Journey: %1 to %2").arg(m_result->departureStation()).arg(m_result->arrivalStation()));
    else
        event.setDisplayLabel(tr("Journey: %1 via %3 to %2").arg(m_result->departureStation()).arg(m_result->arrivalStation()).arg(viaStation));
    event.setDescription(desc);
    event.setStartDateTime(m_result->departureDateTime());
    event.setEndDateTime(m_result->arrivalDateTime());

    emit addCalendarEntryComplete(defaultManager.saveItem(&event));
#endif

    QThread::currentThread()->exit(0);

    // Move back to GUI thread so the deleteLater() callback works (it requires
    // an event loop which is still alive)
    moveToThread(QCoreApplication::instance()->thread());
}
