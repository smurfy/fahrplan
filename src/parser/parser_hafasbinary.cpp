/*******************************************************************************

    This file is a part of Fahrplan for maemo 2009-2012

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

#include "parser_hafasbinary.h"


ParserHafasBinary::ParserHafasBinary(QObject *parent)
{
    Q_UNUSED(parent);
    // baseXmlUrl = "http://reiseauskunft.bahn.de/bin/query.exe";
    // baseSTTableUrl = "http://mobile.bahn.de/bin/mobil/stboard.exe/en";
    // baseUrl = "http://reiseauskunft.bahn.de/bin/query.exe";
    // baseBinaryUrl = "http://reiseauskunft.bahn.de/bin/query.exe/eox";
}

void ParserHafasBinary::searchJourney(const QString &departureStation, const QString &arrivalStation, const QString &viaStation, const QDate &date, const QTime &time, Mode mode, int trainrestrictions)
{
    currentRequestState = FahrplanNS::searchJourneyRequest;

    QString trainrestr = getTrainRestrictionsCodes(trainrestrictions);

    QUrl uri = baseBinaryUrl;
    uri.addQueryItem("start", "Suchen");
    uri.addEncodedQueryItem("REQ0JourneyStopsS0ID", QUrl::toPercentEncoding("A=1@G=" + departureStation));
    uri.addEncodedQueryItem("REQ0JourneyStopsZ0ID", QUrl::toPercentEncoding("A=1@G=" + arrivalStation));

    if (!viaStation.isEmpty()) {
        uri.addQueryItem("REQ0JourneyStops1.0A", "1");
        uri.addQueryItem("REQ0JourneyStops1.0G", viaStation);
    }

    uri.addQueryItem("REQ0JourneyDate", date.toString("dd.MM.yyyy"));
    uri.addQueryItem("REQ0JourneyTime", time.toString("hh:mm"));
    uri.addQueryItem("REQ0HafasSearchForw", QString::number(mode));
    uri.addQueryItem("REQ0JourneyProduct_prod_list_1", trainrestr);
    uri.addQueryItem("h2g-direct", "11");
    sendHttpRequest(uri);
}

void ParserHafasBinary::parseSearchJourney(QNetworkReply *networkReply)
{
    lastJourneyResultList = new JourneyResultList();
    journeyDetailInlineData.clear();

    QByteArray tmpBuffer = networkReply->readAll();
    QByteArray buffer = gzipDecompress(tmpBuffer);

    /*
    QFile file("f://out.txt");
    file.open(QIODevice::WriteOnly);
    file.write(buffer);
    file.close();
*/

    QDataStream hafasData(buffer);
    hafasData.setByteOrder(QDataStream::LittleEndian);
    quint16 hafasVersion;
    hafasData >> hafasVersion;

    if (hafasVersion != 5 && hafasVersion != 6) {
        qWarning()<<"Wrong version of hafas binary data";
        emit errorOccured(tr("An error ocurred with the backend"));
        return;
    }

    qDebug()<<"Binary-Data Version: "<<hafasVersion;

    //Basic data offsets
    quint32 serviceDaysTablePtr;
    quint32 stringTablePtr;
    quint32 stationTablePtr;
    quint32 commentTablePtr;
    quint32 extensionHeaderPtr;
    quint32 extensionHeaderLength;
    quint16 errorCode;

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
    QString tmpString;
    for (int num = 0; num < (serviceDaysTablePtr - stringTablePtr); num++) {
        qint8 c;
        hafasData>>c;
        if (c == 0) {
            strings.insert((num - tmpString.length()), tmpString.trimmed());
            tmpString.clear();
        } else {
            tmpString.append((char)c);
        }
    }

    //Looks ok, parsing
    if (errorCode == 0) {
        hafasData.device()->seek(extensionHeaderPtr + 0x8);
        quint16 seqNr;
        quint16 requestIdPtr;
        quint16 encodingPtr;
        quint32 connectionDetailsPtr;
        quint32 attrsOffset;
        quint16 idPtr;
        hafasData >> seqNr;
        hafasData >> requestIdPtr;
        hafasData >> connectionDetailsPtr;
        hafasData.device()->seek(hafasData.device()->pos() + 16);
        hafasData >> encodingPtr;
        hafasData >> idPtr;
        hafasData >> attrsOffset;
        QString encoding = strings[encodingPtr];
        QString requestId = strings[requestIdPtr];
        QString id = strings[idPtr];

        quint32 connectionAttrsPtr;
        if (extensionHeaderLength >= 0x30) {
            if (extensionHeaderLength < 0x32) {
                qWarning()<<"too short: " + extensionHeaderLength;
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
        qDebug()<<"id:"<<id;
        qDebug()<<connectionAttrsPtr;

        hafasData.device()->seek(connectionDetailsPtr);
        quint16 connectionDetailsVersion;
        hafasData >> connectionDetailsVersion;
        if (connectionDetailsVersion != 1) {
            qWarning("unknown connectionDetailsVersion");
            return;
        }
        hafasData.device()->seek(hafasData.device()->pos() + 2);
        quint16 connectionDetailsIndexOffset;
        quint16 connectionDetailsPartOffset;
        quint16 connectionDetailsPartSize;
        quint16 stopsSize;
        quint16 stopsOffset;
        hafasData >> connectionDetailsIndexOffset;
        hafasData >> connectionDetailsPartOffset;
        hafasData >> connectionDetailsPartSize;
        hafasData >> stopsSize;
        hafasData >> stopsOffset;

        hafasData.device()->seek(0x02);
        quint16 resDeparturePtr;
        quint16 resArrivalPtr;
        quint16 numConnections;
        quint16 dateDays;
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

        for (int iConnection = 0; iConnection < numConnections; iConnection++) {
            hafasData.device()->seek(0x4a + iConnection * 12);
            quint16 serviceDaysTableOffset;
            quint32 partsOffset;
            quint16 numParts;
            quint16 numChanges;
            quint16 durationInt;
            hafasData >> serviceDaysTableOffset;
            hafasData >> partsOffset;
            hafasData >> numParts;
            hafasData >> numChanges;
            hafasData >> durationInt;
            QDateTime durationTime = toTime(durationInt);

            qDebug()<<serviceDaysTableOffset<<partsOffset<<numParts<<numParts<<durationTime.time();

            hafasData.device()->seek(serviceDaysTablePtr + serviceDaysTableOffset);

            quint16 serviceTxtPtr;
            quint16 serviceBitBase;
            quint16 serviceBitLength;
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
            quint16 connectionDetailsOffset;
            hafasData >> connectionDetailsOffset;

            hafasData.device()->seek(connectionDetailsPtr + connectionDetailsOffset);
            quint16 realtimeStatus;
            quint16 delay;
            hafasData >> realtimeStatus;
            hafasData >> delay;

            qDebug()<<"RT"<<realtimeStatus<<delay;

            QString connectionId = "TMPC" + QString::number(iConnection);
            /*
            if (connectionAttrsPtr != 0) {
                hafasData.device()->seek(connectionAttrsPtr + iConnection * 2);
                quint16 connectionAttrsIndex;
                hafasData >> connectionAttrsIndex;
                hafasData.device()->seek(attrsOffset + connectionAttrsIndex * 4);
                while (true)
                {
                    quint16 keyPtr;
                    hafasData >> keyPtr;
                    if (!strings.contains(keyPtr)) {
                        break;
                    }
                    QString key = strings[keyPtr];
                    if (key == "ConnectionId") {
                        quint16 valuePtr;
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

                quint16 plannedDepartureTimeInt;
                hafasData >> plannedDepartureTimeInt;
                QDateTime plannedDepartureTime = toTime(plannedDepartureTimeInt, journeyDate.addDays(connectionDayOffset));
                quint16 plannedDepartureIdx;
                quint16 plannedDeparturePtr;
                hafasData >> plannedDepartureIdx;

                quint16 plannedArrivalTimeInt;
                hafasData >> plannedArrivalTimeInt;
                QDateTime plannedArrivalTime = toTime(plannedArrivalTimeInt, journeyDate.addDays(connectionDayOffset));
                quint16 plannedArrivalIdx;
                quint16 plannedArrivalPtr;
                hafasData >> plannedArrivalIdx;

                quint16 type;
                quint16 lineNamePtr;
                quint16 departurePlatformPtr;
                quint16 arrivalPlatformPtr;
                hafasData >> type;
                hafasData >> lineNamePtr;
                hafasData >> departurePlatformPtr;
                hafasData >> arrivalPlatformPtr;

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

                if (type == 2) {
                    lineNames.append(lineName);
                }

                qDebug()<<type<<lineName<<plannedDepartureTime<<plannedDeparture<<plannedDeparturePosition<<plannedArrivalTime<<plannedArrival<<plannedArrivalPosition;

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
                inlineResults->setDuration(durationTime.time().toString("hh:mm"));
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
                item->setDuration(durationTime.time().toString("hh:mm"));
                item->setMiscInfo("");
                item->setTrainType(lineNames.join(", ").trimmed());
                item->setDepartureTime(inlineResults->getItem(0)->departureDateTime().time().toString("hh:mm"));
                item->setArrivalTime(inlineResults->getItem(inlineResults->itemcount() - 1)->arrivalDateTime().time().toString("hh:mm"));
                if (serviceTxt != "always") {
                    item->setMiscInfo(serviceTxt);
                }
                lastJourneyResultList->appendItem(item);
            }
        }
        emit journeyResult(lastJourneyResultList);
    } else if (errorCode == 8) {
        emit errorOccured(tr("Sorry one station name is to ambiguous"));
    } else {
        emit errorOccured(tr("An error ocurred with the backend"));
    }
}

QDateTime ParserHafasBinary::toTime(quint16 time)
{
    QDateTime tmpDateTime;
    if (time == 0xffff)
        return tmpDateTime;
    int hours = time / 100;
    int minutes = time % 100;
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
    tmpDate.setYMD(1980, 1, 1);
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

