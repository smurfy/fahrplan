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

#include "calendarthreadwrapper.h"

#include <QCoreApplication>
#include <QThread>
#include <QSettings>

#ifdef BUILD_FOR_BLACKBERRY
#   include <bb/pim/calendar/CalendarService>
#   include <bb/pim/calendar/CalendarEvent>
using namespace bb::pim::calendar;
#elif defined(BUILD_FOR_HARMATTAN) || defined(BUILD_FOR_MAEMO_5) || defined(BUILD_FOR_SYMBIAN)
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

}

void CalendarThreadWrapper::addToCalendar()
{
    const QString viaStation = m_result->viaStation().name;
    QSettings settings(FAHRPLAN_SETTINGS_NAMESPACE, "fahrplan2");
    QString calendarEntryTitle;
    QString calendarEntryDesc;

    if (viaStation.isEmpty())
        calendarEntryTitle = tr("%1 to %2").arg(m_result->departureStation().name).arg(m_result->arrivalStation().name);
    else
        calendarEntryTitle = tr("%1 via %3 to %2").arg(m_result->departureStation().name).arg(m_result->arrivalStation().name).arg(viaStation);

    if (!m_result->info().isEmpty()) {
        calendarEntryDesc.append(m_result->info() + "\n");
    }

    for (int i=0; i < m_result->itemcount(); i++) {
        JourneyDetailResultItem *item = m_result->getItem(i);

        calendarEntryDesc.append(item->departureStation().departureDateTime.date().toString("dd.MM.yyyy") + " " + item->departureStation().departureDateTime.time().toString("HH:mm") + " " + item->departureStation().name);
        if (!item->departureInfo().isEmpty()) {
            calendarEntryDesc.append("/" + item->departureInfo().replace(tr("Pl."),"").trimmed());
        }
        calendarEntryDesc.append("\n");
        if (!item->train().isEmpty()) {
            calendarEntryDesc.append("--- " + item->train() + " ---\n");
        }
        calendarEntryDesc.append(item->arrivalStation().arrivalDateTime.date().toString("dd.MM.yyyy") + " " + item->arrivalStation().arrivalDateTime.time().toString("HH:mm") + " " + item->arrivalStation().name);
        if (!item->arrivalInfo().isEmpty()) {
          calendarEntryDesc.append("/" + item->arrivalInfo().replace(tr("Pl."),"").trimmed());
        }
        calendarEntryDesc.append("\n");
        if (!item->info().isEmpty()) {
            calendarEntryDesc.append(item->info() + "\n");
        }
    }

#ifdef BUILD_FOR_BLACKBERRY

    CalendarService service;

    QPair<AccountId, FolderId> folder;

    settings.beginGroup("Calendar");
    folder.first = settings.value("AccountId", -1).toInt();
    if (folder.first >= 0)
        folder.second = settings.value("FolderId", -1).toInt();

    if ((folder.first < 0) || (folder.second < 0))
        folder = service.defaultCalendarFolder();

    CalendarEvent event;
    event.setAccountId(folder.first);
    event.setFolderId(folder.second);
    event.setSubject(calendarEntryTitle);
    event.setStartTime(m_result->departureStation().departureDateTime);
    event.setEndTime(m_result->arrivalStation().arrivalDateTime);
    event.setBody(calendarEntryDesc);
    event.setReminder(-1);

    emit addCalendarEntryComplete(service.createEvent(event) == Result::Success);

#elif defined(BUILD_FOR_HARMATTAN) || defined(BUILD_FOR_MAEMO_5) || defined(BUILD_FOR_SYMBIAN)

    QOrganizerManager defaultManager;
    QOrganizerEvent event;
    event.setDisplayLabel(calendarEntryTitle);
    event.setStartDateTime(m_result->departureStation().departureDateTime);
    event.setEndDateTime(m_result->arrivalStation().arrivalDateTime);
    event.setDescription(calendarEntryDesc);

    QString id = settings.value("Calendar/CollectionId").toString();
    if (!id.isEmpty()) {
        QOrganizerCollectionId collectionId = QOrganizerCollectionId::fromString(id);
        if (!collectionId.isNull())
            event.setCollectionId(collectionId);
    }

    emit addCalendarEntryComplete(defaultManager.saveItem(&event));
#endif

    QThread::currentThread()->exit(0);

    // Move back to GUI thread so the deleteLater() callback works (it requires
    // an event loop which is still alive)
    moveToThread(QCoreApplication::instance()->thread());
}
