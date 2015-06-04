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
#elif defined(BUILD_FOR_SAILFISHOS) && defined(BUILD_FOR_OPENREPOS)
#   include <extendedcalendar.h>
#   include <extendedstorage.h>
#   include <kdatetime.h>
#   include <ksystemtimezone.h>
#endif

QString formatStation(const QDateTime dateTime, const QString &stationName, const QString &info = QString())
{
    QString station;
    if (info.isEmpty()) {
        station = stationName;
    } else {
        //: STATION / PLATFORM
        station = CalendarThreadWrapper::tr("%1 / %2", "STATION / PLATFORM").arg(stationName, info);
    }

    //: DATE TIME   STATION
    return CalendarThreadWrapper::tr("%1 %2   %3", "DATE TIME   STATION").arg(
               // TODO: Don't force QLocale::ShortFormat for date, but make it configurable.
               dateTime.toString(QLocale().dateFormat(QLocale::ShortFormat)),
               // Always use short format for time, else you get something like "12:35:00 t".
               dateTime.toString(QLocale().timeFormat(QLocale::ShortFormat)),
               station);
}

CalendarThreadWrapper::CalendarThreadWrapper(JourneyDetailResultList *result, QObject *parent) :
    QObject(parent), m_result(result)
{
}

CalendarThreadWrapper::~CalendarThreadWrapper()
{

}

void CalendarThreadWrapper::addToCalendar()
{

    const QString viaStation = m_result->viaStation();
    QSettings settings(FAHRPLAN_SETTINGS_NAMESPACE, "fahrplan2");
    QString calendarEntryTitle;
    QString calendarEntryDesc;

    if (viaStation.isEmpty())
        calendarEntryTitle = tr("%1 to %2").arg(m_result->departureStation(),
                                                m_result->arrivalStation());
    else
        calendarEntryTitle = tr("%1 via %3 to %2").arg(m_result->departureStation(),
                                                       m_result->arrivalStation(),
                                                       viaStation);

    if (!m_result->info().isEmpty())
        calendarEntryDesc.append(m_result->info()).append("\n");

    const bool compactFormat = settings.value("compactCalendarEntries", false).toBool();
    for (int i=0; i < m_result->itemcount(); i++) {
        JourneyDetailResultItem *item = m_result->getItem(i);

        const QString train = item->direction().isEmpty()
                              ? item->train()
                              : tr("%1 to %2").arg(item->train(), item->direction());

        if (!compactFormat && !train.isEmpty())
            calendarEntryDesc.append("--- ").append(train).append(" ---\n");

        calendarEntryDesc.append(formatStation(item->departureDateTime(),
                                               item->departureStation(),
                                               item->departureInfo()));
        calendarEntryDesc.append("\n");

        if (compactFormat && !train.isEmpty())
            calendarEntryDesc.append("--- ").append(train).append(" ---\n");

        calendarEntryDesc.append(formatStation(item->arrivalDateTime(),
                                               item->arrivalStation(),
                                               item->arrivalInfo()));
        calendarEntryDesc.append("\n");

        if (!compactFormat) {
            if (!item->info().isEmpty())
                calendarEntryDesc.append(item->info()).append("\n");
            calendarEntryDesc.append("\n");
        }
    }

    if (!compactFormat)
        calendarEntryDesc.append(
            tr("-- \nAdded by Fahrplan. Please, re-check the information before your journey."));

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
    event.setStartTime(m_result->departureDateTime());
    event.setEndTime(m_result->arrivalDateTime());
    event.setBody(calendarEntryDesc);
    event.setReminder(-1);

    emit addCalendarEntryComplete(service.createEvent(event) == Result::Success);

#elif defined(BUILD_FOR_HARMATTAN) || defined(BUILD_FOR_MAEMO_5) || defined(BUILD_FOR_SYMBIAN)

    QOrganizerManager defaultManager;
    QOrganizerEvent event;
    event.setDisplayLabel(calendarEntryTitle);
    event.setStartDateTime(m_result->departureDateTime());
    event.setEndDateTime(m_result->arrivalDateTime());
    event.setDescription(calendarEntryDesc);

    QString id = settings.value("Calendar/CollectionId").toString();
    if (!id.isEmpty()) {
        QOrganizerCollectionId collectionId = QOrganizerCollectionId::fromString(id);
        if (!collectionId.isNull())
            event.setCollectionId(collectionId);
    }

    emit addCalendarEntryComplete(defaultManager.saveItem(&event));
  #elif defined(BUILD_FOR_SAILFISHOS) && defined(BUILD_FOR_OPENREPOS)

    mKCal::ExtendedCalendar::Ptr calendar = mKCal::ExtendedCalendar::Ptr ( new mKCal::ExtendedCalendar( QLatin1String( "UTC" ) ) );
    mKCal::ExtendedStorage::Ptr storage = mKCal::ExtendedCalendar::defaultStorage( calendar );
    if (storage->open()) {
        QString uid = settings.value("Calendar/notebookUID").toString();
        mKCal::Notebook::Ptr notebook = storage->notebook(uid);

        if (!notebook) {
            notebook = storage->defaultNotebook();
        }

        if (notebook) {
            KCalCore::Event::Ptr event = KCalCore::Event::Ptr( new KCalCore::Event() );
            event->setSummary(calendarEntryTitle);
            event->setDescription(calendarEntryDesc);
            event->setDtStart( KDateTime(m_result->departureDateTime()) );
            event->setDtEnd( KDateTime(m_result->arrivalDateTime()) );
            calendar->addEvent( event, notebook->uid() );
            storage->save();
            emit addCalendarEntryComplete(true);
        } else {
            emit addCalendarEntryComplete(false);
        }
    } else {
        emit addCalendarEntryComplete(false);
    }
#else
    emit addCalendarEntryComplete(false);
#endif

    QThread::currentThread()->exit(0);

    // Move back to GUI thread so the deleteLater() callback works (it requires
    // an event loop which is still alive)
    moveToThread(QCoreApplication::instance()->thread());
}
