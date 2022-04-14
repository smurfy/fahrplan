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

#include "calendar_sfos_wrapper.h"

#include <QCoreApplication>
#include <QThread>
#include <QSettings>
#include <QDesktopServices>
#include <QTemporaryFile>
#include <QStandardPaths>
#include <QTextStream>
#include <QDateTime>
#include <QDir>
#include <QUrl>
#include <QFile>
#include <QTextDocument>
//#   include <extendedcalendar.h>
//#   include <extendedstorage.h>
//#   include <kdatetime.h>
//#   include <ksystemtimezone.h>

// > 4 #   include <KCalendarCore/Event>
// > 4 #   include <KCalendarCore/ICalFormat>


QString formatStations(const QDateTime dateTime, const QString &stationName, const QString &info = QString())
{
    QString station;
    station = stationName;

    if (info.isEmpty()) {
        station = stationName;
    } else {
        //: STATION / PLATFORM
        station = CalendarSfosWrapper::tr("  %1 / %2", "STATION / PLATFORM").arg(stationName, info);
    }

    QString dSpec = "MM.dd";
    QString tSpec = "hh:mm:ss";

    //: DATE TIME   STATION
    return CalendarSfosWrapper::tr("  %1 %2   %3", "DATE TIME   STATION").arg(
               // TODO: Don't force QLocale::ShortFormat for date, but make it configurable.
               //dateTime.toString(QLocale().dateFormat(QLocale::ShortFormat)),
               dateTime.toString(dSpec),
               // Always use short format for time, else you get something like "12:35:00 t".
               dateTime.toString(QLocale().timeFormat(QLocale::ShortFormat)),
               station);
}

CalendarSfosWrapper::CalendarSfosWrapper(JourneyDetailResultList *result, QObject *parent) :
    QObject(parent), m_result(result)
{
}

CalendarSfosWrapper::~CalendarSfosWrapper()
{

}

QString CalendarSfosWrapper::iterInfos(JourneyDetailResultItem *item)
{
return formatStations(item->departureDateTime(), item->departureStation(), item->departureInfo()) +
        "---------------------------" +
formatStations(item->arrivalDateTime(), item->arrivalStation(), item->arrivalInfo());
}

void CalendarSfosWrapper::addToCalendar()
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
            calendarEntryDesc.append(" # ").append(train).append("  # --> ");

        calendarEntryDesc.append(formatStations(item->departureDateTime(),
                                               item->departureStation(),
                                               item->departureInfo()) );

        if (compactFormat && !train.isEmpty())
            calendarEntryDesc.append(" # ").append(train).append(" # --> ");

        calendarEntryDesc.append(formatStations(item->arrivalDateTime(),
                                               item->arrivalStation(),
                                               item->arrivalInfo()));

        /*if (!compactFormat) {
            if (!item->info().isEmpty())
                calendarEntryDesc.append(item->info());

        }*/
    }

    /*if (!compactFormat)
        calendarEntryDesc.append(
            tr("#### Added by Fahrplan. Please, re-check the information before your journey. ####\n"));*/

/* > 4
    KCalendarCore::Event::Ptr event( new KCalendarCore::Event() );
    event->setSummary(calendarEntryTitle);
    event->setDescription(calendarEntryDesc);
    event->setDtStart( m_result->departureDateTime() );
    event->setDtEnd( m_result->arrivalDateTime() );
    KCalendarCore::ICalFormat format;


   < 4
    KCalCore::Event::Ptr event = KCalCore::Event::Ptr( new KCalCore::Event() );
    event->setSummary(calendarEntryTitle);
    event->setDescription(calendarEntryDesc);
    event->setDtStart( KDateTime(m_result->departureDateTime()) );
    event->setDtEnd( KDateTime(m_result->arrivalDateTime()) );
*/
    QTemporaryFile *tmpFile = new QTemporaryFile(
                QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) +
                QDir::separator() + "fahrplan-event-XXXXXX.ics",
                this);

                // destructed and file deleted with this object


    /*for (int i=0; i < m_result->itemcount(); i++) {
        JourneyDetailResultItem *item = m_result->getItem(i);
        aDesc.append(this->iterInfos(item));
    }*/
    // construct our ical doc
    QString tSpec = "yyyyMMddThhmmss";
    QString dDate = m_result->departureDateTime().toString(tSpec);
    QString aDate =  m_result->arrivalDateTime().toString(tSpec);

    // The info elements have fragments which don't render properly.
    calendarEntryDesc.remove(QRegExp("<[^>]*>"));

    QString aDesc  =  "BEGIN:VCALENDAR\n";
    aDesc.append("PRODID:-//IDN smurfy.de//Fahrplan 2.0.38//EN\n");
    aDesc.append("VERSION:2.0\n");
    aDesc.append("BEGIN:VEVENT\n");
    aDesc.append("DTSTART:" +  dDate + "\n");
    aDesc.append("DTEND:" +  aDate +  "\n");
    aDesc.append("SUMMARY:" + calendarEntryTitle + "\n");
    aDesc.append("DESCRIPTION:" + calendarEntryDesc + "\n");
    aDesc.append("END:VEVENT\n");
    aDesc.append("END:VCALENDAR");

    if (tmpFile->open())
    {
        QTextStream stream( tmpFile );
        //stream << aDesc;
        /* This works until you get char \n in Desc */
          stream << "BEGIN:VCALENDAR" << "\n"
            << "PRODID:-//IDN smurfy.de//Fahrplan 2.0.38//EN" << "\n"
            << "VERSION:2.0" << "\n"
            << "BEGIN:VEVENT" << "\n"
            << "DTSTART:" <<  dDate << "\n"
            << "DTEND:" <<  aDate <<  "\n"
            << "SUMMARY:" << calendarEntryTitle << "\n"
            << "DESCRIPTION:" << calendarEntryDesc << "\n"
            << "END:VEVENT" << "\n"
            << "END:VCALENDAR";
        tmpFile->close();
        qDebug() << "Opening" << tmpFile->fileName();
        if ( !QDesktopServices::openUrl(QUrl::fromLocalFile(tmpFile->fileName())) ) 
	{
            qWarning() << "QDesktopServices::openUrl fails!";
            emit addCalendarEntryComplete(false);
        } else {
            emit addCalendarEntryComplete(true);
        }
    }
}
