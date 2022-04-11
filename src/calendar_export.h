#ifndef CALENDAR_EXPORT_H
#define CALENDAR_EXPORT_H

#include <QObject>
#include <QTemporaryFile>
#include <QDir>
#include <QUrl>
#include <QDesktopServices>

#include <QDebug>

class CalendarExport : public QObject
{
    Q_OBJECT
public:
    explicit CalendarExport(QObject *parent = nullptr);

    Q_INVOKABLE void shareCalEvent(const QString &event);

signals:

};

#endif // CALENDAR_EXPORT_H
