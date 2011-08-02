/*******************************************************************************

    This file is a part of Fahrplan for maemo 2009-2010

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

#ifndef PARSER_HAFASXML_H
#define PARSER_HAFASXML_H

#include <QObject>
#include <QtXmlPatterns/QXmlQuery>
#include <QXmlResultItems>
#include "parser_abstract.h"

class ParserHafasXml : public ParserAbstract
{
    Q_OBJECT
public:
    explicit ParserHafasXml(QObject *parent = 0);

public slots:
    void findStationsByName(QString stationName);
    void findStationsByCoordinates(qreal longitude, qreal latitude);
    bool supportsGps();

protected:
    void parseStationsByName(QNetworkReply *networkReply);
    void parseStationsByCoordinates(QNetworkReply *networkReply);

private:
    QString baseUrl;
};

#endif // PARSER_HAFASXML_H
