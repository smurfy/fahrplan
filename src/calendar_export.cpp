#include <QDesktopServices>
#include <QDirIterator>
#include <QFile>
#include <QTextStream>
#include <QSettings>
#include "calendar_export.h"

CalendarExport::CalendarExport(QObject *parent) : QObject(parent)
{

}

void CalendarExport::shareCalEvent(const QString &event)
{

/*
 * #ifdef defined(BUILD_FOR_SAILFISHOS) && defined(BUILD_FOR_OPENREPOS)

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
*/
    QTemporaryFile *tmpFile = new QTemporaryFile(
        QDir::tempPath() + QDir::separator() + "event-XXXXXX.ics",
        this); // destructed and file deleted with this object

      if (tmpFile->open()) {
        QTextStream stream( tmpFile );
        stream << "BEGIN:VCALENDAR" << '\n'
               << "VERSION:2.0" << '\n'
               << "CALSCALE:GREGORIAN" << '\n'
               << "PRODID:-//BRK Aubing//EN" << '\n'
               << "METHOD:PUBLISH" << '\n'
               << event << '\n'
               << "END:VCALENDAR" << '\n';
        tmpFile->close();

        qDebug() << "Opening" << tmpFile->fileName();
        if (!QDesktopServices::openUrl(QUrl("file://" + tmpFile->fileName(), QUrl::TolerantMode))) {
          qWarning() << "QDesktopServices::openUrl fails!";
        }
    }
}
