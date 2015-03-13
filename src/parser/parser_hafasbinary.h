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

#ifndef PARSER_HAFASBINARY_H
#define PARSER_HAFASBINARY_H

#include <QObject>
#include <QDataStream>
#include "parser_hafasxml.h"

class ParserHafasBinary : public ParserHafasXml
{
    Q_OBJECT
public:
    explicit ParserHafasBinary(QObject *parent = 0);

    static QString getName() { return "HafasBinary"; }
    virtual QString name() { return getName(); }
    virtual QString shortName() { return getName(); }

    void searchJourney(const Station &departureStation, const Station &viaStation, const Station &arrivalStation, const QDateTime &dateTime, Mode mode, int trainrestrictions);
    void searchJourneyEarlier();
    void searchJourneyLater();

protected:
    QString baseBinaryUrl;
    void parseSearchJourney(QNetworkReply *networkReply);
    void parseSearchLaterJourney(QNetworkReply *networkReply);
    void parseSearchEarlierJourney(QNetworkReply *networkReply);
    QDate toDate(quint16 date);
    QDateTime toTime(quint16 time, QDate baseDate);
    QDateTime toTime(quint16 time);
    QString formatDuration(QDateTime durationTime);
    QString errorString(int error) const;

private:
    mutable QHash<int, QString> stringCache;
    QString getString(const QByteArray &data, int index, QTextCodec *dataCodec) const;
};

#endif // PARSER_HAFASBINARY_H
