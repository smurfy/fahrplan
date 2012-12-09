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

    baseXmlUrl = "http://mobile.bahn.de/bin/mobil/query.exe";
    baseSTTableUrl = "http://mobile.bahn.de/bin/mobil/stboard.exe/en";
    baseUrl = "http://mobile.bahn.de/bin/mobil/query.exe";
    baseBinaryUrl = "http://reiseauskunft.bahn.de/bin/query.exe";
    STTableMode = 1;
}

void ParserHafasBinary::parseSearchJourney(QNetworkReply *networkReply)
{
    switch (searchJourneyRequestData.progress) {
    case 1:
        parseSearchJourneyPart1(networkReply);
        break;
    case 2:
        parseSearchJourneyPart2(networkReply);
        break;
    }
}

void ParserHafasBinary::parseSearchJourneyPart1(QNetworkReply *networkReply)
{
    ParserHafasXmlExternalIds extIds = parseExternalIds(networkReply->readAll());

    if (!extIds.departureId.isEmpty() && !extIds.arrivalId.isEmpty()) {

        currentRequestState = FahrplanNS::searchJourneyRequest;
        searchJourneyRequestData.progress = 2;

        QString trainrestr = getTrainRestrictionsCodes(searchJourneyRequestData.trainrestrictions);

        QUrl uri = baseBinaryUrl;
        uri.addQueryItem("start", "Suchen");
        uri.addEncodedQueryItem("REQ0JourneyStopsS0ID", QUrl::toPercentEncoding("A=1@L=" + extIds.departureId));
        uri.addEncodedQueryItem("REQ0JourneyStopsZ0ID", QUrl::toPercentEncoding("A=1@L=" + extIds.arrivalId));

        //Todo: via station :)

        uri.addQueryItem("REQ0JourneyDate", searchJourneyRequestData.date.toString("dd.MM.yyyy"));
        uri.addQueryItem("REQ0JourneyTime", searchJourneyRequestData.time.toString("hh:mm"));
        uri.addQueryItem("REQ0HafasSearchForw", QString::number(searchJourneyRequestData.mode));
        uri.addQueryItem("REQ0JourneyProduct_prod_list_1", trainrestr);
        uri.addQueryItem("h2g-direct", "11");
        sendHttpRequest(uri);
    }
}

void ParserHafasBinary::parseSearchJourneyPart2(QNetworkReply *networkReply)
{
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
        hafasData >> seqNr;
        hafasData >> requestIdPtr;
        hafasData >> connectionDetailsPtr;
        hafasData.device()->seek(hafasData.device()->pos() + 16);
        hafasData >> encodingPtr;
        QString encoding = strings[encodingPtr];
        QString requestId = strings[requestIdPtr];

        qDebug()<<"seqNr:"<<seqNr;
        qDebug()<<"reqId:"<<requestId;
        qDebug()<<"encoding:"<<encoding;

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
        QString resDeparture = strings[resDeparturePtr];
        QString resArrival = strings[resArrivalPtr];
        QDate journeyDate;
        journeyDate.setYMD(1980, 1, 1);
        journeyDate = journeyDate.addDays(dateDays);
        qDebug()<<resDeparture<<resArrival<<numConnections<<journeyDate;

        for (int iConnection = 0; iConnection < numConnections; iConnection++) {
            hafasData.device()->seek(0x4a + iConnection * 12);
            quint16 serviceDaysTableOffset;
            quint32 partsOffset;
            quint16 numParts;
            quint16 numChanges;
            hafasData >> serviceDaysTableOffset;
            hafasData >> partsOffset;
            hafasData >> numParts;
            hafasData >> numChanges;
            //time

            qDebug()<<serviceDaysTableOffset<<partsOffset<<numParts<<numParts;
        }
    }
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

