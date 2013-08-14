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

#include "parser_hafasbinary.h"

#include <QNetworkReply>

#if defined(BUILD_FOR_QT5)
    #include <QUrlQuery>
#endif

ParserHafasBinary::ParserHafasBinary(QObject *parent) :
    ParserHafasXml(parent)
{
    // baseXmlUrl = "http://reiseauskunft.bahn.de/bin/query.exe";
    // baseSTTableUrl = "http://mobile.bahn.de/bin/mobil/stboard.exe/en";
    // baseUrl = "http://reiseauskunft.bahn.de/bin/query.exe";
    // baseBinaryUrl = "http://reiseauskunft.bahn.de/bin/query.exe/eox";
}

void ParserHafasBinary::searchJourney(const Station &departureStation, const Station &viaStation, const Station &arrivalStation, const QDateTime &dateTime, Mode mode, int trainrestrictions)
{
    if (currentRequestState != FahrplanNS::noneRequest) {
        return;
    }

    currentRequestState = FahrplanNS::searchJourneyRequest;
    hafasContext.seqNr = "";
    lastJourneyResultList = NULL;

    QString trainrestr = getTrainRestrictionsCodes(trainrestrictions);

    QUrl uri = baseBinaryUrl;
#if defined(BUILD_FOR_QT5)
    QUrlQuery query;
#else
    QUrl query;
#endif
    query.addQueryItem("start", "Suchen");
    query.addQueryItem("REQ0JourneyStopsS0ID", departureStation.id.toString());
    query.addQueryItem("REQ0JourneyStopsZ0ID", arrivalStation.id.toString());

    if (viaStation.id.isValid()) {
        query.addQueryItem("REQ0JourneyStops1.0ID", viaStation.id.toString());
    }

    query.addQueryItem("REQ0JourneyDate", dateTime.toString("dd.MM.yyyy"));
    query.addQueryItem("REQ0JourneyTime", dateTime.toString("hh:mm"));
    query.addQueryItem("REQ0HafasSearchForw", mode == Arrival ? "0" : "1");
    query.addQueryItem("REQ0JourneyProduct_prod_list_1", trainrestr);
    query.addQueryItem("h2g-direct", "11");

#if defined(BUILD_FOR_QT5)
    uri.setQuery(query);
#else
    uri.setQueryItems(query.queryItems());
#endif

    sendHttpRequest(uri);
}

void ParserHafasBinary::parseSearchJourney(QNetworkReply *networkReply)
{
    lastJourneyResultList = new JourneyResultList();
    journeyDetailInlineData.clear();

    QByteArray tmpBuffer = networkReply->readAll();

    if (tmpBuffer.count() < 10 || tmpBuffer.at(0) != 0x1f) {
        qWarning()<<"Bad data in response (can not find gzip magic number)";
        emit errorOccured(tr("An error ocurred with the backend"));
        return;
    }

    QByteArray buffer = gzipDecompress(tmpBuffer);

    /*
    QFile file("/tmp/out.txt");
    file.open(QIODevice::WriteOnly);
    file.write(buffer);
    file.close();
*/

    QDataStream hafasData(buffer);
    hafasData.setByteOrder(QDataStream::LittleEndian);

    qint16 hafasVersion;
    hafasData >> hafasVersion;

    if (hafasVersion != 5 && hafasVersion != 6) {
        qWarning()<<"Wrong version of hafas binary data";
        emit errorOccured(tr("An error ocurred with the backend"));
        return;
    }

    qDebug()<<"Binary-Data Version: "<<hafasVersion;

    //Basic data offsets
    qint32 serviceDaysTablePtr;
    qint32 stringTablePtr;
    qint32 stationTablePtr;
    qint32 commentTablePtr;
    qint32 extensionHeaderPtr;
    qint32 extensionHeaderLength;
    qint16 errorCode;

    hafasData.device()->seek(0x20);
    hafasData >> serviceDaysTablePtr;
    hafasData >> stringTablePtr;
    hafasData.device()->seek(0x36);
    hafasData >> stationTablePtr;
    hafasData >> commentTablePtr;
    hafasData.device()->seek(0x46);
    hafasData >> extensionHeaderPtr;
    hafasData.device()->seek(extensionHeaderPtr);
    hafasData >> extensionHeaderLength;
    hafasData.device()->seek(extensionHeaderPtr + 16);
    hafasData >> errorCode;

    //Debug data offsets
    qDebug()<<serviceDaysTablePtr<<stringTablePtr;
    qDebug()<<stationTablePtr<<commentTablePtr;
    qDebug()<<extensionHeaderPtr<<extensionHeaderLength;
    qDebug()<<errorCode;

    //Read strings
    hafasData.device()->seek(stringTablePtr);
    QMap<int, QString> strings;
    QByteArray tmpString;
    for (int num = 0; num < (serviceDaysTablePtr - stringTablePtr); num++) {
        qint8 c;
        hafasData>>c;
        if (c == 0) {
            strings.insert((num - tmpString.length()), QString::fromLatin1(tmpString.trimmed()));
            tmpString.clear();
        } else {
            tmpString.append((char)c);
        }
    }

    //Looks ok, parsing
    if (errorCode == 0) {
        hafasData.device()->seek(extensionHeaderPtr + 0x8);
        qint16 seqNr;
        qint16 requestIdPtr;
        qint16 encodingPtr;
        qint32 connectionDetailsPtr;
        qint32 attrsOffset;
        qint16 ldPtr;
        hafasData >> seqNr;
        hafasData >> requestIdPtr;
        hafasData >> connectionDetailsPtr;
        hafasData.device()->seek(hafasData.device()->pos() + 16);
        hafasData >> encodingPtr;
        hafasData >> ldPtr;
        hafasData >> attrsOffset;
        QString encoding = strings[encodingPtr];
        QString requestId = strings[requestIdPtr];
        QString ld = strings[ldPtr];

        qint32 connectionAttrsPtr;
        if (extensionHeaderLength >= 0x30) {
            if (extensionHeaderLength < 0x32) {
                qWarning() << "too short:" << extensionHeaderLength;
                return;
            }
            hafasData.device()->seek(extensionHeaderPtr + 0x2c);
            hafasData >> connectionAttrsPtr;
        } else {
            connectionAttrsPtr = 0;
        }

        qDebug()<<"seqNr:"<<seqNr;
        qDebug()<<"reqId:"<<requestId;
        qDebug()<<"encoding:"<<encoding;
        qDebug()<<"ld:"<<ld;
        qDebug()<<connectionAttrsPtr;

        hafasData.device()->seek(connectionDetailsPtr);
        quint16 connectionDetailsVersion;
        hafasData >> connectionDetailsVersion;
        if (connectionDetailsVersion != 1) {
            qWarning("unknown connectionDetailsVersion");
            return;
        }
        hafasData.device()->seek(hafasData.device()->pos() + 2);
        qint16 connectionDetailsIndexOffset;
        qint16 connectionDetailsPartOffset;
        qint16 connectionDetailsPartSize;
        qint16 stopsSize;
        qint16 stopsOffset;
        hafasData >> connectionDetailsIndexOffset;
        hafasData >> connectionDetailsPartOffset;
        hafasData >> connectionDetailsPartSize;
        hafasData >> stopsSize;
        hafasData >> stopsOffset;

        hafasData.device()->seek(0x02);
        qint16 resDeparturePtr;
        qint16 resArrivalPtr;
        qint16 numConnections;
        qint16 dateDays;
        hafasData >> resDeparturePtr;
        hafasData.device()->seek(hafasData.device()->pos() + 2 + 2 + 4 + 4);
        hafasData >> resArrivalPtr;
        hafasData.device()->seek(hafasData.device()->pos() + 2 + 2 + 4 + 4);
        hafasData >> numConnections;
        hafasData.device()->seek(hafasData.device()->pos() + 4 + 4);
        hafasData >> dateDays;
        QDate journeyDate = toDate(dateDays);
        QString resDeparture = strings[resDeparturePtr];
        QString resArrival = strings[resArrivalPtr];

        lastJourneyResultList->setDepartureStation(resDeparture);
        lastJourneyResultList->setArrivalStation(resArrival);
        lastJourneyResultList->setTimeInfo(journeyDate.toString());

        qDebug()<<resDeparture<<resArrival<<numConnections<<journeyDate;

        QMultiMap<QDateTime, JourneyResultItem*> journeyResultsByArrivalMap;

        for (int iConnection = 0; iConnection < numConnections; iConnection++) {
            hafasData.device()->seek(0x4a + iConnection * 12);
            qint16 serviceDaysTableOffset;
            qint32 partsOffset;
            qint16 numParts;
            qint16 numChanges;
            qint16 durationInt;
            hafasData >> serviceDaysTableOffset;
            hafasData >> partsOffset;
            hafasData >> numParts;
            hafasData >> numChanges;
            hafasData >> durationInt;
            QDateTime durationTime = toTime(durationInt);
            qDebug()<<serviceDaysTableOffset<<partsOffset<<numParts<<numParts<<durationTime;

            hafasData.device()->seek(serviceDaysTablePtr + serviceDaysTableOffset);

            qint16 serviceTxtPtr;
            qint16 serviceBitBase;
            qint16 serviceBitLength;
            hafasData >> serviceTxtPtr;
            hafasData >> serviceBitBase;
            hafasData >> serviceBitLength;
            QString serviceTxt = strings[serviceTxtPtr];

            int connectionDayOffset = serviceBitBase * 8;
            for (int i = 0; i < serviceBitLength; i++)
            {
                qint8 serviceBits;
                hafasData >> serviceBits;
                if (serviceBits == 0)
                {
                    connectionDayOffset += 8;
                    continue;
                }
                while ((serviceBits & 0x80) == 0)
                {
                    serviceBits = serviceBits << 1;
                    connectionDayOffset++;
                }
                break;
            }

            qDebug()<<serviceTxt<<connectionDayOffset;

            hafasData.device()->seek(connectionDetailsPtr + connectionDetailsIndexOffset + iConnection * 2);
            qint16 connectionDetailsOffset;
            hafasData >> connectionDetailsOffset;

            /*
            hafasData.device()->seek(connectionDetailsPtr + connectionDetailsOffset);
            qint16 realtimeStatus;
            qint16 delay;
            hafasData >> realtimeStatus;
            hafasData >> delay;

            qDebug()<<"RT"<<realtimeStatus<<delay;
            */

            QString connectionId = "TMPC" + QString::number(iConnection);
            /*
            if (connectionAttrsPtr != 0) {
                hafasData.device()->seek(connectionAttrsPtr + iConnection * 2);
                qint16 connectionAttrsIndex;
                hafasData >> connectionAttrsIndex;
                hafasData.device()->seek(attrsOffset + connectionAttrsIndex * 4);
                while (true)
                {
                    qint16 keyPtr;
                    hafasData >> keyPtr;
                    if (!strings.contains(keyPtr)) {
                        break;
                    }
                    QString key = strings[keyPtr];
                    if (key == "ConnectionId") {
                        qint16 valuePtr;
                        hafasData >> valuePtr;
                        connectionId = strings[valuePtr];
                        break;
                    } else {
                        hafasData.device()->seek(hafasData.device()->pos() + 2);
                    }
                }
            }*/

            qDebug()<<"conId"<<connectionId;
            QStringList lineNames;

            JourneyDetailResultList *inlineResults = new JourneyDetailResultList();

            for (int iPart = 0; iPart < numParts; iPart++) {

                JourneyDetailResultItem *inlineItem = new JourneyDetailResultItem();

                hafasData.device()->seek(0x4a + partsOffset + iPart * 20);

                qint16 plannedDepartureTimeInt;
                hafasData >> plannedDepartureTimeInt;
                QDateTime plannedDepartureTime = toTime(plannedDepartureTimeInt, journeyDate.addDays(connectionDayOffset));
                qint16 plannedDepartureIdx;
                qint16 plannedDeparturePtr;
                hafasData >> plannedDepartureIdx;

                qint16 plannedArrivalTimeInt;
                hafasData >> plannedArrivalTimeInt;
                QDateTime plannedArrivalTime = toTime(plannedArrivalTimeInt, journeyDate.addDays(connectionDayOffset));
                qint16 plannedArrivalIdx;
                qint16 plannedArrivalPtr;
                hafasData >> plannedArrivalIdx;

                qint16 type;
                qint16 lineNamePtr;
                qint16 departurePlatformPtr;
                qint16 arrivalPlatformPtr;
                qint16 partAttrIndex;
                hafasData >> type;
                hafasData >> lineNamePtr;
                hafasData >> departurePlatformPtr;
                hafasData >> arrivalPlatformPtr;
                hafasData >> partAttrIndex;

                QString lineName = strings[lineNamePtr];
                QString plannedDeparturePosition = strings[departurePlatformPtr];
                QString plannedArrivalPosition = strings[arrivalPlatformPtr];

                if (plannedDeparturePosition == "---") {
                    plannedDeparturePosition = "";
                }
                if (plannedArrivalPosition == "---") {
                    plannedArrivalPosition = "";
                }

                //Getting Station Names last because we seeking in the buffer
                hafasData.device()->seek((plannedDepartureIdx * 14) + stationTablePtr);
                hafasData >> plannedDeparturePtr;
                hafasData.device()->seek((plannedArrivalIdx * 14) + stationTablePtr);
                hafasData >> plannedArrivalPtr;

                QString plannedDeparture = strings[plannedDeparturePtr];
                QString plannedArrival = strings[plannedArrivalPtr];

                hafasData.device()->seek(attrsOffset + partAttrIndex * 4);

                QString category = "";

                while (true)
                {
                    qint16 tmpTxtPtr;
                    hafasData >> tmpTxtPtr;
                    QString key = strings[tmpTxtPtr];

                    if (key.isEmpty() || key == "---") {
                        break;
                    } else if (key == "Direction") {
                        //directionStr = strings.read(is);
                        hafasData.device()->seek(hafasData.device()->pos() + 2);
                    } else if (key == "Class") {
                        //lineClass = Integer.parseInt(strings.read(is));
                        hafasData.device()->seek(hafasData.device()->pos() + 2);
                    } else if (key == "Category") {
                         hafasData >> tmpTxtPtr;
                         category = strings[tmpTxtPtr];
                        //lineCategory = strings.read(is);
                         hafasData.device()->seek(hafasData.device()->pos() + 2);
                    } else if (key == "Operator") {
                        //lineOperator = strings.read(is);
                         hafasData.device()->seek(hafasData.device()->pos() + 2);
                    } else {
                        hafasData.device()->seek(hafasData.device()->pos() + 2);
                    }
                 }

                if (type == 2 && !category.isEmpty()) {
                    lineNames.append(category);
                }

                qDebug()<<type<<lineName<<plannedDepartureTime<<plannedDeparture<<plannedDeparturePosition<<plannedArrivalTime<<plannedArrival<<plannedArrivalPosition<<category;

                inlineItem->setDepartureDateTime(plannedDepartureTime);
                inlineItem->setDepartureStation(plannedDeparture);
                inlineItem->setDepartureInfo(plannedDeparturePosition);
                inlineItem->setArrivalDateTime(plannedArrivalTime);
                inlineItem->setArrivalStation(plannedArrival);
                inlineItem->setArrivalInfo(plannedArrivalPosition);
                inlineItem->setTrain(lineName);
                inlineResults->appendItem(inlineItem);
            }

            if (inlineResults->itemcount() > 0) {
                inlineResults->setId(connectionId);
                inlineResults->setDuration(formatDuration(durationTime));
                inlineResults->setDepartureStation(inlineResults->getItem(0)->departureStation());
                inlineResults->setArrivalStation(inlineResults->getItem(inlineResults->itemcount() - 1)->arrivalStation());
                inlineResults->setDepartureDateTime(inlineResults->getItem(0)->departureDateTime());
                inlineResults->setArrivalDateTime(inlineResults->getItem(inlineResults->itemcount() - 1)->arrivalDateTime());
                journeyDetailInlineData.append(inlineResults);

                lineNames.removeDuplicates();

                JourneyResultItem *item = new JourneyResultItem();
                item->setDate(journeyDate);
                item->setId(connectionId);
                item->setTransfers(QString::number(numChanges));
                item->setDuration(formatDuration(durationTime));
                item->setMiscInfo("");
                item->setTrainType(lineNames.join(", ").trimmed());
                item->setDepartureTime(inlineResults->getItem(0)->departureDateTime().time().toString(tr("hh:mm")));
                item->setArrivalTime(inlineResults->getItem(inlineResults->itemcount() - 1)->arrivalDateTime().time().toString(tr("hh:mm")));
                journeyResultsByArrivalMap.insert(inlineResults->getItem(inlineResults->itemcount() - 1)->arrivalDateTime(), item);
            }
        }

        QList<JourneyResultItem*> journeyResultsByArrivalList = journeyResultsByArrivalMap.values();
        Q_FOREACH(JourneyResultItem *item, journeyResultsByArrivalList) {
            lastJourneyResultList->appendItem(item);
        }

        hafasContext.seqNr = QString::number(seqNr);
        hafasContext.ld = ld;
        hafasContext.ident = requestId;

        emit journeyResult(lastJourneyResultList);
    } else {
        emit errorOccured(errorString(errorCode));
    }
}

void ParserHafasBinary::parseSearchLaterJourney(QNetworkReply *networkReply)
{
    parseSearchJourney(networkReply);
}

void ParserHafasBinary::parseSearchEarlierJourney(QNetworkReply *networkReply)
{
    parseSearchJourney(networkReply);
}


void ParserHafasBinary::searchJourneyLater()
{
    if (hafasContext.seqNr.isEmpty()) {
        emit errorOccured(tr("Internal error occured, going later is not possible"));
        return;
    }

    if (currentRequestState != FahrplanNS::noneRequest) {
        return;
    }

    currentRequestState = FahrplanNS::searchJourneyLaterRequest;

    QUrl uri = baseBinaryUrl;
#if defined(BUILD_FOR_QT5)
    QUrlQuery query;
#else
    QUrl query;
#endif
    query.addQueryItem("seqnr", hafasContext.seqNr);
    query.addQueryItem("ident", hafasContext.ident);
    query.addQueryItem("REQ0HafasScrollDir", "1");
    query.addQueryItem("h2g-direct", "11");
    if (!hafasContext.ld.isEmpty()) {
        query.addQueryItem("ld", hafasContext.ld);
    }
#if defined(BUILD_FOR_QT5)
    uri.setQuery(query);
#else
    uri.setQueryItems(query.queryItems());
#endif
    sendHttpRequest(uri);
}

void ParserHafasBinary::searchJourneyEarlier()
{
    if (hafasContext.seqNr.isEmpty()) {
        emit errorOccured(tr("Internal error occured, going earlier is not possible"));
        return;
    }

    if (currentRequestState != FahrplanNS::noneRequest) {
        return;
    }

    currentRequestState = FahrplanNS::searchJourneyEarlierRequest;

    QUrl uri = baseBinaryUrl;
#if defined(BUILD_FOR_QT5)
    QUrlQuery query;
#else
    QUrl query;
#endif
    query.addQueryItem("seqnr", hafasContext.seqNr);
    query.addQueryItem("ident", hafasContext.ident);
    query.addQueryItem("REQ0HafasScrollDir", "2");
    query.addQueryItem("h2g-direct", "11");
    if (!hafasContext.ld.isEmpty()) {
        query.addQueryItem("ld", hafasContext.ld);
    }
#if defined(BUILD_FOR_QT5)
    uri.setQuery(query);
#else
    uri.setQueryItems(query.queryItems());
#endif
    sendHttpRequest(uri);
}

QString ParserHafasBinary::formatDuration(QDateTime durationTime)
{
    QString tmpDuration = durationTime.time().toString("hh:mm");
    QDateTime refDate;
    refDate.setDate(QDate::currentDate());
    int days = refDate.daysTo(durationTime);
    if (days > 0) {
        tmpDuration = QString::number(days) + "d " + tmpDuration;
    }
    return tmpDuration;
}

QDateTime ParserHafasBinary::toTime(quint16 time)
{
    QDateTime tmpDateTime;
    if (time == 0xffff)
        return tmpDateTime;
    int hours = time / 100;
    int minutes = time % 100;
    tmpDateTime.setDate(QDate::currentDate());
    return tmpDateTime.addSecs(((hours * 60) + minutes) * 60);
}

QDateTime ParserHafasBinary::toTime(quint16 time, QDate baseDate)
{
    QDateTime tmpDateTime;
    if (time == 0xffff)
        return tmpDateTime;
    tmpDateTime.setDate(baseDate);
    int hours = time / 100;
    int minutes = time % 100;
    return tmpDateTime.addSecs(((hours * 60) + minutes) * 60);
}

QDate ParserHafasBinary::toDate(quint16 date)
{
    QDate tmpDate;
    tmpDate.setDate(1980, 1, 1);
    return tmpDate.addDays(date - 1);
}

QByteArray ParserHafasBinary::gzipDecompress(QByteArray compressData)
{
    //decompress GZIP data

    //strip header and trailer
    compressData.remove(0, 10);
    compressData.chop(12);

    const int buffersize = 16384;
    quint8 buffer[buffersize];

    z_stream cmpr_stream;
    cmpr_stream.next_in = (unsigned char *)compressData.data();
    cmpr_stream.avail_in = compressData.size();
    cmpr_stream.total_in = 0;

    cmpr_stream.next_out = buffer;
    cmpr_stream.avail_out = buffersize;
    cmpr_stream.total_out = 0;

    cmpr_stream.zalloc = Z_NULL;
    cmpr_stream.zalloc = Z_NULL;

    if( inflateInit2(&cmpr_stream, -8 ) != Z_OK) {
        qDebug() << "cmpr_stream error!";
    }

    QByteArray uncompressed;
    do {
        int status = inflate( &cmpr_stream, Z_SYNC_FLUSH );

        if(status == Z_OK || status == Z_STREAM_END) {
            uncompressed.append(QByteArray::fromRawData((char *)buffer, buffersize - cmpr_stream.avail_out));
            cmpr_stream.next_out = buffer;
            cmpr_stream.avail_out = buffersize;
        } else {
            inflateEnd(&cmpr_stream);
        }

        if(status == Z_STREAM_END) {
            inflateEnd(&cmpr_stream);
            break;
        }
    } while(cmpr_stream.avail_out == 0);
    return uncompressed;
}

QString ParserHafasBinary::errorString(int error) const
{
    // Some error code descriptions can be found here:
    // http://code.google.com/p/public-transport-enabler/source/browse/enabler/src/de/schildbach/pte/AbstractHafasProvider.java
    switch (error) {
    case 1:
        return tr("Your session has expired. Please, perform the search again.");
    case 8:
        return tr("One of the station names is too ambiguous.");
    case 890:
        return tr("No connections have been found that correspond to your request.");
    case 899:
        return tr("There was an unsuccessful or incomplete search due to a timetable change.");
    case 9240:
        return tr("Unfortunately there was no route found.");
    case 9360:
        return tr("Unfortunately your connection request can currently not be processed. It might be that entered date is not inside the timetable period.");
    default:
        return tr("Unknown error ocurred with the backend (error %1)").arg(error);
    }
}
