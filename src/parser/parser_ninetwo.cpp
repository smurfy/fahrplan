/****************************************************************************
 * *
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

#include "parser_ninetwo.h"

#include <QUrl>
#include <QNetworkReply>
#include <QDomDocument>
#include <QTextStream>
#ifdef BUILD_FOR_QT5
#   include <QUrlQuery>
#else
#   define setQuery(q) setQueryItems(q.queryItems())
#endif
#include <qmath.h>

#define BASE_URL "https://9292.nl/en"

inline qreal deg2rad(qreal deg)
{
    return deg * 3.141592653589793238463 / 180;
}

inline int distance(qreal lat1, qreal lon1, qreal lat2, qreal lon2)
{
    const qreal sdLat = qSin(deg2rad(lat2 - lat1) / 2);
    const qreal sdLon = qSin(deg2rad(lon2 - lon1) / 2);
    const qreal cLat1 = qCos(deg2rad(lat1));
    const qreal cLat2 = qCos(deg2rad(lat2));
    const qreal a = sdLat * sdLat + sdLon * sdLon * cLat1 * cLat2;
    const qreal c = 2 * qAtan2(qSqrt(a), qSqrt(1 - a));

    // 6371 km - Earth radius.
    // We return distance in meters, thus * 1000.
    return qRound(6371.0 * c * 1000);
}

ParserNinetwo::ParserNinetwo(QObject *parent):ParserAbstract(parent)
{
    lastCoordinates.isValid = false;
}

void ParserNinetwo::getTimeTableForStation(const Station &currentStation,
                                           const Station &,
                                           const QDateTime &,
                                           ParserAbstract::Mode,
                                           int trainrestrictions)
{
    QUrl uri(QString(BASE_URL "/locations/%1/departure-times").arg(currentStation.id.toString()));

    #if defined(BUILD_FOR_QT5)
    QUrlQuery query;
    #else
    QUrl query;
    #endif

    query.addQueryItem("lang", 	"en-GB");
    uri.setQuery(query);

    timetableRestrictions  =  trainrestrictions;
    sendHttpRequest(uri);
    currentRequestState=FahrplanNS::getTimeTableForStationRequest;
}

void ParserNinetwo::findStationsByName(const QString &stationName)
{
    // TODO: do a bogus search, and use the "did you perhapss mean ..." response
    // bacause it seems that some places do not get suggested
    QUrl url(BASE_URL "/suggest");
    qDebug() << "FINDBYNAME" << stationName << url;

    #if defined(BUILD_FOR_QT5)
    QUrlQuery query;
    #else
    QUrl query;
    #endif
    query.addQueryItem("userInput", stationName);
    url.setQuery(query);

    sendHttpRequest(url);
    currentRequestState=FahrplanNS::stationsByNameRequest;
}

void ParserNinetwo::findStationsByCoordinates(qreal longitude, qreal latitude)
{
    // No longer supported.
    // TODO: use a free geocoding api, and then search on
    // postal code


    QUrl uri(BASE_URL "locations");

    #if defined(BUILD_FOR_QT5)
    QUrlQuery query;
    #else
    QUrl query;
    #endif
    query.addQueryItem("lang", 	"en-GB");
    query.addQueryItem("latlong", QString("%1,%2").arg(latitude).arg(longitude));
    query.addQueryItem("includestation", "true");
    uri.setQuery(query);

    lastCoordinates.isValid = true;
    lastCoordinates.latitude = latitude;
    lastCoordinates.longitude = longitude;
    currentRequestState = FahrplanNS::stationsByCoordinatesRequest;

    sendHttpRequest(uri);
}

void ParserNinetwo::searchJourney(const Station &departureStation,
                                  const Station &viaStation,
                                  const Station &arrivalStation,
                                  const QDateTime &dateTime,
                                  const ParserAbstract::Mode mode,
                                  int trainrestrictions)
{
    lastsearch.from=departureStation;
    lastsearch.to=arrivalStation;
    lastsearch.restrictions=trainrestrictions;
    lastsearch.via=viaStation;
    QUrl url(BASE_URL "/journeyadvice");

    #if defined(BUILD_FOR_QT5)
    QUrlQuery postData;
    #else
    QUrl postData;
    #endif

    /*
	// this is for /journeyadvice-page
    query.addQueryItem("from", departureStation.id.toString());
    query.addQueryItem("to", arrivalStation.id.toString());
    query.addQueryItem("searchType", mode == Mode::Arrival ? "arrival" : "departure");
    query.addQueryItem("dateTime", dateTime.toString("yyyy-MM-ddThhmm"));
    query.addQueryItem("extraInterchangeTime", 0);
    query.addQueryItem("flexStop", "False");
    query.addQueryItem("flexAddress", "False");
    query.addQueryItem("before", QString::number(-1));
    query.addQueryItem("bfter", QString::number(5));
    */

    // This post request is inconsistent AF. here and there it uses "true"/"false"
    // and other places it uses "on"/"off"

     static const char* const o[] = {"off", "on"};
     postData.addQueryItem("fromText","");
     postData.addQueryItem("fromLock","false");
     postData.addQueryItem("fromOriginal","");
     postData.addQueryItem("fromRef",departureStation.id.toString());
     postData.addQueryItem("fromRefOriginal","");
     postData.addQueryItem("viaText", viaStation.valid ? viaStation.name : "");
     postData.addQueryItem("viaOriginal","");
     postData.addQueryItem("viaRef", viaStation.valid ? viaStation.id.toString() : "");
     postData.addQueryItem("viaRefOriginal","");
     postData.addQueryItem("toText","");
     postData.addQueryItem("toLock","false");
     postData.addQueryItem("toOriginal","");
     postData.addQueryItem("toRef",arrivalStation.id.toString());
     postData.addQueryItem("toRefOriginal","");
     postData.addQueryItem("date",dateTime.toString("dd-MM-yyyy"));
     postData.addQueryItem("time",dateTime.toString("hh:mm"));
     postData.addQueryItem("dateTime",dateTime.toString("yyyy-MM-ddhh:mm"));
     postData.addQueryItem("searchType", (mode == Mode::Arrival) ? "aankomst" : "vertrek");
     postData.addQueryItem("extraInterchangeTime","0");
     postData.addQueryItem("travelByBus",o[trainrestrictions != restrictions::trainsOnly ]);
     postData.addQueryItem("travelByTrain","on");
     postData.addQueryItem("travelByTram",o[trainrestrictions != restrictions::trainsOnly ]);
     postData.addQueryItem("travelBySubway",o[trainrestrictions != restrictions::trainsOnly ]);
     postData.addQueryItem("travelByFerry",o[trainrestrictions == restrictions::all]);
     postData.addQueryItem("planWithAccessibility","false");

    #if defined(BUILD_FOR_QT5) || defined(BUILD_FOR_QT6)
    QByteArray data = postData.query().toUtf8().replace(' ', '+') ;
	#else
    QByteArray data = postData.encodedQuery().toUtf8().replace(' ', '+') ;
    #endif

    qDebug() << "SEARCH JOURNEYS" << url << postData.query();
    sendHttpRequest(url, data);
    currentRequestState=FahrplanNS::searchJourneyRequest;
}

void ParserNinetwo::searchJourneyLater()
{
    searchJourney(lastsearch.from, lastsearch.via, lastsearch.to, lastsearch.lastOption, Departure , lastsearch.restrictions);

}

void ParserNinetwo::searchJourneyEarlier()
{
    QDateTime time = lastsearch.firstOption.addSecs(-30*60);
    searchJourney(lastsearch.from, lastsearch.via, lastsearch.to,time, Departure, lastsearch.restrictions);
}

void ParserNinetwo::getJourneyDetails(const QString &id)
{
    if(cachedResults.contains(id))
        emit journeyDetailsResult(cachedResults.value(id));
}

QStringList ParserNinetwo::getTrainRestrictions()
{
    QStringList restrictions;
    restrictions << tr("All");
    restrictions << tr("Only trains");
    restrictions << tr("All, except ferry");
    return restrictions;
}

bool sortByTimeLessThan(const TimetableEntry &first, const TimetableEntry &second)
{
    if (first.time == second.time)
        return first.trainType < second.trainType;
    else
        return first.time < second.time;
}

void ParserNinetwo::parseTimeTable(QNetworkReply *networkReply)
{
    TimetableEntriesList result;
    QByteArray allData = networkReply->readAll();
    qDebug() << "REPLY:>>>>>>>>>>>>\n" << allData;

    QVariantMap doc = parseJson(allData);
    if (doc.isEmpty()) {
        emit errorOccured(tr("Cannot parse reply from the server"));
        return;
    }

    QVariantList tabs = doc.value("tabs").toList();
    QString currentStation(doc.value("location").toMap().value("name").toString());

    QVariantList::const_iterator i;
    for (i = tabs.constBegin(); i != tabs.constEnd(); ++i) {
        QVariantMap tab = i->toMap();
        QString type = tab.value("id").toString();
        QVariantList departures = tab.value("departures").toList();
        switch(timetableRestrictions){
            case all:
            default:
                break;
            case trainsOnly:
                if(type!="train")
                    continue;
            break;
            case noFerry:
                if(type=="ferry")//not sure if this ever happens
                    continue;
            break;
        }

        QVariantList::const_iterator j;
        for (j = departures.constBegin(); j != departures.constEnd(); ++j) {
            QVariantMap departure = j->toMap();
            TimetableEntry entry;
            entry.currentStation=currentStation;
            entry.destinationStation = departure.value("destinationName").toString();
            entry.time = QTime::fromString(departure.value("time").toString(), "HH:mm");
            QString via(departure.value("viaNames").toString());
            if (!via.isEmpty())
                entry.destinationStation = tr("%1 via %2").arg(entry.destinationStation, via);
            QStringList info;
            const QString rtStatus = departure.value("realtimeState").toString();
            if (rtStatus == "ontime") {
                info << tr("On-Time");
            } else if (rtStatus == "late") {
                const QString rtMessage = departure.value("realtimeText").toString().trimmed();
                if (!rtMessage.isEmpty())
                    info << QString("<span style=\"color:#b30;\">%1</span>").arg(rtMessage);
            }
            const QString remark = departure.value("remark").toString();
            if (!remark.isEmpty())
                info << remark;
            entry.miscInfo = info.join("<br />");

            entry.platform = departure.value("platform").toString();

            QString train = departure.value("mode").toMap().value("name").toString();
            const QString service = departure.value("service").toString();
            if (!service.isEmpty())
                train.append(" ").append(service);
            entry.trainType = train;

            result.append(entry);
        }
    }

    // Departures / arrivals are grouped by transportation type,
    // while we want them sorted by departure / arrival time.
    std::sort(result.begin(), result.end(), sortByTimeLessThan);

    emit timetableResult(result);

}

void ParserNinetwo::parseStationsByName(QNetworkReply *networkReply)
{
    qDebug() << __PRETTY_FUNCTION__ << "PARSING STATIONS";
    QByteArray allData = networkReply->readAll();
    qDebug() << "REPLY:>>>>>>>>>>>>\n" << allData;

    QVariantMap doc = parseJson(allData);
    if (doc.isEmpty()) {
        qWarning() << __PRETTY_FUNCTION__ << "empty response";
        emit errorOccured(tr("Cannot parse reply from the server"));
        return;
    }

    QVariantList stations = doc.value("locations").toList();

    StationsList result;

    QVariantList::const_iterator i;
    for (i = stations.constBegin(); i != stations.constEnd(); ++i) {
        QVariantMap station = i->toMap();
        Station s;
        //s.name=QString("[%1]%2").arg(station.value("type").toString(), station.value("name").toString());
        const QString name = station.value("Displayname").toString();
        const QString place = station.value("Region").toString();
        s.miscInfo = station.value("EnglishSubType",station.value("SubType")).toString();

        s.name = QString("%3 %1 (%2)").arg(name, place, s.miscInfo);

        s.id = station.value("Url").toString();
        result.append(s);
    }

    emit stationsResult(result);
}

void ParserNinetwo::parseStationsByCoordinates(QNetworkReply *networkReply)
{
    parseStationsByName(networkReply);

    lastCoordinates.isValid = false;
}

void ParserNinetwo::parseSearchJourney(QNetworkReply *networkReply)
{

    std::unique_ptr<JourneyResultList> resultList{ new JourneyResultList};

    qDebug() << __LINE__ << "PARSING JOURNEYS" ;
    //qDebug() << "REPLY:>>>>>>>>>>>>\n" << allData;
//    QVariantMap data = parseJson(allData);

    //QDomDocument slider { data.value("JourneySliderBlock").toString() };
    //qDebug() << "DETAIL" << data.value("JourneyDetailBlock");
    //QDomDocument detail { data.value("JourneyDetailBlock").toString().replace("\\r\\n", "\n").replace("\\\"", "\"" ).replace("&amp;", "&") };

    QTextStream stream{networkReply};

    QString line;
    while(!stream.atEnd())
    {
        stream.readLineInto(&line);
        if(line.contains("Journey.Legs"))
        {
            // use qDom struff only on this one element
            // to extract the "value" and to do urlDecode
            QDomDocument detail;
            QString error;
            detail.setContent(line, &error);
            if(!error.isEmpty())
            {
                qWarning() << "error parsing html" << "\n" << error;
            }
            auto input = detail.documentElement();
            JourneyResultItem* option = parseJourneyJson(input.attribute("value"));

            if(option)
                resultList->appendItem(option);
            else
                qWarning() << "parsing json faied";
        }
    }


    //for (int i = 0; i < divs.count(); i++)
    //{
    //    auto div = divs.item(i);
    //    if(!div.isElement()) continue;
    //    if(!div.toElement().hasAttribute("data-icscontent")) continue;
    //    qDebug() << "found ics data";
    //    auto* res = parseJourneyICS(div.toElement().attribute("data-icscontent"));
    //    if(res)
    //        resultList->appendItem(res);
    //}
    //lastsearch.lastOption=departure;


    emit journeyResult(resultList.release());
}

void ParserNinetwo::parseSearchLaterJourney(QNetworkReply *)
{

}

void ParserNinetwo::parseSearchEarlierJourney(QNetworkReply *)
{

}

void ParserNinetwo::parseJourneyDetails(QNetworkReply *)
{
    //should never happen
}


// Not finished and untested
// but might be a start, if the json way breaks:w

JourneyResultItem* ParserNinetwo::parseJourneyICS(QString const & ics)
{
    std::unique_ptr<JourneyResultItem> item {new JourneyResultItem};
    qDebug() << "parsing ICS" << ics;
    QStringList lines = ics.split("\r\n");

    QDateTime startTime{};
    QString description{};

    for(QString line: lines)
    {
        auto sp = line.split(':');
        if(sp.length() < 2) continue;
        QString key = sp[0];
        QString value = sp[1];
        if(key == "DTSTART"    ) startTime   = QDateTime::fromString(value, Qt::DateFormat::ISODate);
        if(key == "DESCRIPTION") description = value;
    }

    if(description.isEmpty()) return nullptr;

    QStringList descLines {description.split("\\n") };

    int walkCount = 0;

    auto iter = descLines.begin();
    std::unique_ptr<JourneyDetailResultList> detail{ new JourneyDetailResultList };

    while(iter != descLines.end())
    {
        // legs are separated by a blank line
        QStringList legLines;
        do
        {
            legLines.append(iter->toLower());
            iter++;
        }
        while(iter != descLines.end() && *iter != "");

        JourneyDetailResultItem* leg = new JourneyDetailResultItem;

        QString verb; // walk, train, metro etc
        QString direction;
        QString train;// metro B
        {
            QStringList words{ legLines[0].split(' ', Qt::SkipEmptyParts) };
            verb = words[0];
            if(verb == "walk")
                walkCount++;
            int directionIndex = words.indexOf("direction");
            if(directionIndex >= 0 && directionIndex+1 < words.count())
            {
                direction = words.takeAt(directionIndex + 1);
                words.takeAt(directionIndex); // "direction"
            }
            train = words.join(' ');
        }

        for(QString line : legLines)
        {
            if(line.startsWith("departure") || line.startsWith("arrival"))
            {
                QStringList words = line.split(' ', Qt::SkipEmptyParts);
                if(words.count() < 3) return nullptr;
                QString depArr = words.takeFirst();
                QTime time = QTime::fromString(words.takeFirst(), "hh:mm");
                QDateTime dateTime {startTime.date(), time};
                QString platform;
                int platformIndex = words.indexOf("platform");
                if(platformIndex >= 0 && platformIndex+1 < words.count())
                {
                    platform = words.takeAt(platformIndex + 1);
                    words.takeAt(platformIndex); // "platform"
                }
                QString station = words.join(' ');
                leg->setProperty((depArr + "DateTime").toStdString().c_str(), dateTime);
                leg->setProperty((depArr + "Station").toStdString().c_str(), station);
                leg->setProperty((depArr + "Station").toStdString().c_str(), station);
            }
        }
        detail->appendItem(leg);
    }
    if(detail->itemcount() == 0)
        return nullptr;
    static int count = 0;
    QString id = QString::number(count ++);
    cachedResults.insert(id, detail.release());

    item->setDepartureTime(detail->getItem(0)->departureDateTime().toString("hh:mm"));
    item->setArrivalTime  (detail->getItem(detail->itemcount()-1)->arrivalDateTime().toString("hh:mm"));
    item->setTransfers(QString::number(detail->itemcount() -walkCount-1) + " +" + QString::number(walkCount) );
    item->setId(id);

    return item.release();
}

JourneyResultItem * ParserNinetwo::parseJourneyJson(const QString& jsonData)
{
    // this really helps :)
    //std::ofstream("/tmp/journey.json") << jsonData.toStdString();
    std::unique_ptr<JourneyResultItem>     response {new JourneyResultItem};
    std::unique_ptr<JourneyDetailResultList> detail {new JourneyDetailResultList};


    // wrap it in an object, bcause it is a naked list
    auto json = parseJson("{\"legs\":" + jsonData.toUtf8() + "}");
    if(json["legs"].type() != QVariant::List)
        return nullptr;
    QVariantList legs = json["legs"].toList();

    QSet<QString> transportTypes;
    int walkCount = 0;
    for(auto const & legv: legs)
    {
        if(legv.type() != QVariant::Map)
        {
            qWarning() << "non-map leg detected";
            continue;
        }
        QVariantMap leg = legv.toMap();
        QString mode = leg["Mode"].toMap()["ModeType"].toString();
        transportTypes.insert(mode);
        QString modeName = leg["Mode"].toMap()["Name"].toString();
        QString direction = leg["Destination"].toString();

        // eg. B for metro line b
        QString service = leg["Service"].toString();
        QString company = leg["Operator"].toMap()["Name"].toString();

        if(mode == "walk")
            walkCount++;
        QVariantList calls = leg["Calls"].toList();

        QString departureStation = calls[0].toMap()["Location"].toMap()["Cluster-Type"].toString() + " "
                                 + calls[0].toMap()["Location"].toMap()["Name"].toString();
        QString departurePlatform = calls[0].toMap()["Platform"].toString();
        QDateTime departureTime = QDateTime::fromString(calls[0].toMap()["Departure"].toString(), Qt::ISODate);
        int const lastCall = calls.count()-1;
        QString arrivalStation = calls[lastCall].toMap()["Location"].toMap()["Cluster-Type"].toString() + " "
                               + calls[lastCall].toMap()["Location"].toMap()["Name"].toString();
        QDateTime arrivalTime = QDateTime::fromString(calls[lastCall].toMap()["Arrival"].toString(), Qt::ISODate);
        QString arrivalPlatform = calls[lastCall].toMap()["Platform"].toString();

        JourneyDetailResultItem* item = new JourneyDetailResultItem;

        item->setDirection(direction);

        if(mode == "walk")
            item->setTrain(tr("Walk"));
        else
            item->setTrain(modeName + ": " + service);

        item->setArrivalDateTime(arrivalTime);
        item->setArrivalStation(arrivalStation);
        if(!arrivalPlatform.isEmpty())
            item->setArrivalInfo(QString(tr("Pl. %1")).arg(arrivalPlatform));

        item->setDepartureDateTime(departureTime);
        item->setDepartureStation(departureStation);
        if(!departurePlatform.isEmpty())
            item->setDepartureInfo(QString(tr("Pl. %1")).arg(departurePlatform));

        detail->appendItem(item);
    }

    if(detail->itemcount() == 0)
        return nullptr;

    static int count = 0;
    count++;
    QString id = QString::number(count);

    detail->setId(id);

    response->setId(id);
    response->setDate(detail->getItem(0)->departureDateTime().date());
    response->setDepartureTime(detail->getItem(0)->departureDateTime().toString("hh:mm"));
    response->setArrivalTime  (detail->getItem(detail->itemcount()-1)->arrivalDateTime().toString("hh:mm"));
    response->setTransfers(QString::number(detail->itemcount()-1 -walkCount) + " +" + QString::number(walkCount) );

    {
        QString tt;
        for(QString const & t: transportTypes)
            tt += t + ',';
        tt.resize(tt.size()-1);
        response->setTrainType(tt);
    }
    int durationMins = (
            detail->getItem(detail->itemcount()-1)->arrivalDateTime().toMSecsSinceEpoch() -
            detail->getItem(0)->departureDateTime().toMSecsSinceEpoch()
        )/1000 / 60;
    response->setDuration(
        QString::number(durationMins / 60) + ":" + QString::number(durationMins % 60)
    );


    cachedResults.insert(id, detail.release());
    return response.release();
}
