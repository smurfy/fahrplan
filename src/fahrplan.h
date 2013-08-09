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

#ifndef FAHRPLAN_H
#define FAHRPLAN_H

#include "parser/parser_definitions.h"
#include "parser/parser_abstract.h"

#include <QObject>
#include <QSettings>
#include <QStringList>

class FahrplanBackendManager;
class FahrplanParserThread;
class Favorites;
class Fahrplan : public QObject
{
    Q_OBJECT

    Q_PROPERTY(FahrplanParserThread *parser READ parser)
    Q_PROPERTY(Favorites *favorites READ favorites CONSTANT)
    Q_PROPERTY(QString parserName READ parserName)
    Q_PROPERTY(QString version READ getVersion CONSTANT)

    Q_ENUMS(StationType)

    public:
        enum StationType {
            DepartureStation,
            ViaStation,
            ArrivalStation,
            CurrentStation,
            DirectionStation
        };

        explicit Fahrplan(QObject *parent = 0);
        FahrplanParserThread *parser();
        Favorites *favorites() const;
        QString parserName();
        QString getVersion();

    public slots:
        QStringList getParserList();
        void setParser(int index);
        void storeSettingsValue(const QString &key, const QString &value);
        QString getSettingsValue(const QString &key, const QString &defaultValue);
        void addJourneyDetailResultToCalendar(JourneyDetailResultList *result);

    signals:
        void parserStationsResult(StationsResultList *result);
        void parserJourneyResult(JourneyResultList *result);
        void parserJourneyDetailsResult(JourneyDetailResultList *result);
        void parserTimeTableResult(TimeTableResultList *result);
        void parserErrorOccured(const QString &msg);
        void parserChanged(const QString &name, int index);
        void favoritesChanged(const QStringList &favorites);
        void addCalendarEntryComplete(bool success);

    private slots:
        void onParserChanged(const QString &name, int index);
        void bindParserSignals();

    private:
        static FahrplanBackendManager *m_parser_manager;
        static Favorites *m_favorites_manager;
        QSettings *settings;
};
#endif // FAHRPLAN_H
