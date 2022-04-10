#include <QDesktopServices>
#include <QDirIterator>
#include <QFile>
#include <QTextStream>
#include <QSettings>
/*
QTemporaryFile *tmpFile = new QTemporaryFile(
QDir::tempPath() + QDir::separator() + "event-XXXXXX.ics",
    this); // destructed and file deleted with this object

  if (tmpFile->open()) {
    QTextStream stream( tmpFile );
    stream << "BEGIN:VCALENDAR" << '\n'
           << "..." << '\n'
           << "END:VCALENDAR" << '\n';
    tmpFile->close();

    qDebug() << "Opening" << tmpFile->fileName();
    if (!QDesktopServices::openUrl("file://" + tmpFile->fileName())) {
    // if (!QDesktopServices::openUrl(QUrl("file://" + tmpFile->fileName(), QUrl::TolerantMode)))
      qWarning() << "QDesktopServices::openUrl fails!";
    }
  }
*/
