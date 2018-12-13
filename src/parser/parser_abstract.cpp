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

#include "parser_abstract.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSslError>
#include <QTimer>

#include <zlib.h>

#ifdef BUILD_FOR_QT5
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#else
#include <QScriptEngine>
#include <QScriptValue>
#endif

#ifdef BUILD_FOR_UBUNTU

// in order to build it, you need to install libcurl4-gnutls-dev:armhf in the chroot
// libcurl itself is already preinstalled on the phones, and thus does not need to be in the click package
// see 3rdparty/README_CURL.txt for details

#include "3rdparty/QtCUrl/QtCUrl.h"
#include "3rdparty/qcustomnetworkreply/qcustomnetworkreply.h"
#endif

ParserAbstract::ParserAbstract(QObject *parent) :
    QObject(parent)
{
    NetworkManager = new QNetworkAccessManager(this);
    connect(NetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(networkReplyFinished(QNetworkReply*)));
    connect(NetworkManager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
            this, SLOT(networkReplySslErrors(QNetworkReply*,QList<QSslError>)));

    currentRequestState = FahrplanNS::noneRequest;

    requestTimeout = new QTimer();

    connect(requestTimeout, SIGNAL(timeout()), this, SLOT(networkReplyTimedOut()));

    userAgent = "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:13.0) Gecko/20100101 Firefox/13.0";

    sendHttpRequestViaCurl = false;
}

ParserAbstract::~ParserAbstract()
{
    delete requestTimeout;
    delete NetworkManager;
}

void ParserAbstract::networkReplyFinished(QNetworkReply *networkReply)
{
    FahrplanNS::curReqStates internalRequestState = currentRequestState;

    disconnect(lastRequest, SIGNAL(downloadProgress(qint64,qint64)), 0, 0);
    requestTimeout->stop();

    lastRequest = NULL;

    //We overwrite the currentRequestState to noneRequest here, because this allows us to set a new one
    //if needed inside the parser
    currentRequestState = FahrplanNS::noneRequest;

    if (internalRequestState == FahrplanNS::stationsByNameRequest) {
        parseStationsByName(networkReply);
    } else if (internalRequestState == FahrplanNS::stationsByCoordinatesRequest) {
        parseStationsByCoordinates(networkReply);
    } else if (internalRequestState == FahrplanNS::searchJourneyRequest) {
        parseSearchJourney(networkReply);
    } else if (internalRequestState == FahrplanNS::searchJourneyLaterRequest) {
        parseSearchLaterJourney(networkReply);
    } else if (internalRequestState == FahrplanNS::searchJourneyEarlierRequest) {
        parseSearchEarlierJourney(networkReply);
    } else if (internalRequestState == FahrplanNS::journeyDetailsRequest) {
        parseJourneyDetails(networkReply);
    } else if (internalRequestState == FahrplanNS::getTimeTableForStationRequest) {
        parseTimeTable(networkReply);
    } else {
        qDebug()<<"Current request unhandled!";
    }
}

void ParserAbstract::cancelRequest()
{
    requestTimeout->stop();
    if (lastRequest) {
        lastRequest->abort();
    }
}

void ParserAbstract::sendHttpRequest(QUrl url, QByteArray data, const QList<QPair<QByteArray,QByteArray> > &additionalHeaders)
{
    // workaround for Ubuntu / HTTPS with bahn.de
    // here we use cUrl instead
    #ifdef BUILD_FOR_UBUNTU
    if (sendHttpRequestViaCurl)
    {
       ParserAbstract::sendHttpRequestCUrl(url, data, additionalHeaders);
       return;
    }
    #endif

    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QLatin1String("application/x-www-form-urlencoded"));
    request.setRawHeader("User-Agent", userAgent.toLatin1());
    request.setRawHeader("Cache-Control", "no-cache");
    for (QList<QPair<QByteArray,QByteArray> >::ConstIterator it = additionalHeaders.constBegin(); it != additionalHeaders.constEnd(); ++it)
        request.setRawHeader(it->first, it->second);
    if (!acceptEncoding.isEmpty()) {
        request.setRawHeader("Accept-Encoding", acceptEncoding);
    }

    if (data.isNull()) {
        lastRequest = NetworkManager->get(request);
    } else {
        lastRequest = NetworkManager->post(request, data);
    }

    requestTimeout->start(30000);

    connect(lastRequest, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(networkReplyDownloadProgress(qint64,qint64)));
}

QVariant ParserAbstract::parseJson(const QByteArray &json) const
{
    QVariant doc;
#ifdef BUILD_FOR_QT5
    doc = QJsonDocument::fromJson(json).toVariant();
#else
    QString utf8(QString::fromUtf8(json));

    // Validation of JSON according to RFC 4627, section 6
    QString tmp(utf8);
    if (tmp.replace(QRegExp("\"(\\\\.|[^\"\\\\])*\""), "")
           .contains(QRegExp("[^,:{}\\[\\]0-9.\\-+Eaeflnr-u \\n\\r\\t]")))
        return doc;

    QScriptEngine *engine = new QScriptEngine();
    doc = engine->evaluate("(" + utf8 + ")").toVariant();
    delete engine;
#endif

    return doc;
}

#ifdef BUILD_FOR_QT5
QByteArray ParserAbstract::serializeToJson(const QVariant& doc) const
{
    return QJsonDocument::fromVariant(doc).toJson(QJsonDocument::Indented);
}
#else
QByteArray toJson(const QVariant& value)
{
    if (value.isNull())
        return "null";
    switch (value.type()) {
    case QVariant::String:
        return "\"" + value.toString().replace("\"", "\\\"").toUtf8() + "\"";
    case QVariant::Int:
        return QString::number(value.toInt()).toUtf8();
    case QVariant::Double:
        return QString::number(value.toDouble()).toUtf8();
    case QVariant::Bool:
        if (value.toBool())
            return "true";
        else
            return "false";
    case QVariant::Map:
    {
        const QVariantMap& map(value.toMap());
        QStringList members;
        Q_FOREACH (const QString& key, map.keys()) {
            members.append("\"" + key + "\": " + toJson(map.value(key)));
        }
        return "{" + members.join(", ").toUtf8() + "}";
    }
    case QVariant::List:
    {
        QStringList elements;
        Q_FOREACH (const QVariant& value, value.toList()) {
            elements.append(toJson(value));
        }
        return "[" + elements.join(", ").toUtf8() + "]";
    }
    default:
        qDebug() << "Failed to serialize" << value << "to JSON";
        return "";
    }
}

QByteArray ParserAbstract::serializeToJson(const QVariant& doc) const
{
    return toJson(doc);
}
#endif

void ParserAbstract::networkReplyDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    Q_UNUSED(bytesReceived)
    Q_UNUSED(bytesTotal)
    requestTimeout->stop();
    requestTimeout->start(30000);
}

void ParserAbstract::networkReplyTimedOut()
{
    cancelRequest();
    emit errorOccured(tr("Request timed out."));
}

void ParserAbstract::networkReplySslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
    if (lastRequest != reply) {
        qDebug() << Q_FUNC_INFO << "Wrong network reply - ignoring. Expected" << lastRequest
                 << "received" << reply;
        return;
    }

    foreach (const QSslError &error, errors) {
        qDebug() << Q_FUNC_INFO << error;
        if (ignoredSslErrors.contains(error.error())) {
            reply->ignoreSslErrors();
            return;
        }
    }
}

void ParserAbstract::sendHttpRequest(QUrl url)
{
    sendHttpRequest(url, NULL);
}

bool ParserAbstract::supportsGps()
{
    return false;
}

bool ParserAbstract::supportsVia()
{
    return false;
}

bool ParserAbstract::supportsTimeTable()
{
    return false;
}

bool ParserAbstract::supportsTimeTableDirection()
{
    return false;
}

QStringList ParserAbstract::getTrainRestrictions()
{
    QStringList result;
    return result;
}

void ParserAbstract::getTimeTableForStation(const Station &currentStation, const Station &directionStation, const QDateTime &dateTtime, Mode mode, int trainrestrictions)
{
    qDebug() << "ParserAbstract::getTimeTableForStation";
    Q_UNUSED(currentStation);
    Q_UNUSED(directionStation);
    Q_UNUSED(dateTtime);
    Q_UNUSED(mode);
    Q_UNUSED(trainrestrictions);
}

void ParserAbstract::parseTimeTable(QNetworkReply *networkReply)
{
   Q_UNUSED(networkReply);
   qDebug() << "ParserAbstract::parseTimeTable";
}

void ParserAbstract::findStationsByName(const QString &stationName)
{
    Q_UNUSED(stationName);
}

void ParserAbstract::findStationsByCoordinates(qreal longitude, qreal latitude)
{
    Q_UNUSED(longitude);
    Q_UNUSED(latitude);
}

 void ParserAbstract::parseStationsByName(QNetworkReply *networkReply)
 {
    Q_UNUSED(networkReply);
    qDebug() << "ParserAbstract::parseStationsByName";
 }

 void ParserAbstract::parseStationsByCoordinates(QNetworkReply *networkReply)
 {
     Q_UNUSED(networkReply);
     qDebug() << "ParserAbstract::parseStationsByCoordinates";
 }

 void ParserAbstract::searchJourney(const Station &departureStation, const Station &viaStation, const Station &arrivalStation, const QDateTime &dateTime, Mode mode, int trainrestrictions)
 {
     Q_UNUSED(departureStation);
     Q_UNUSED(viaStation);
     Q_UNUSED(arrivalStation);
     Q_UNUSED(dateTime);
     Q_UNUSED(mode);
     Q_UNUSED(trainrestrictions);
     qDebug() << "ParserAbstract::searchJourney";
 }

 void ParserAbstract::parseSearchJourney(QNetworkReply *networkReply)
 {
     Q_UNUSED(networkReply);
     qDebug() << "ParserAbstract::parseSearchJourney";
 }

 void ParserAbstract::searchJourneyLater()
 {
     qDebug() << "ParserAbstract::searchJourneyLater";
 }

 void ParserAbstract::searchJourneyEarlier()
 {
     qDebug() << "ParserAbstract::searchJourneyEarlier";
 }

 void ParserAbstract::parseSearchLaterJourney(QNetworkReply *networkReply)
 {
     Q_UNUSED(networkReply);
     qDebug() << "ParserAbstract::parseSearchLaterJourney";
 }

 void ParserAbstract::parseSearchEarlierJourney(QNetworkReply *networkReply)
 {
     Q_UNUSED(networkReply);
     qDebug() << "ParserAbstract::parseSearchEarlierJourney";
 }

 void ParserAbstract::getJourneyDetails(const QString &id)
 {
     Q_UNUSED(id);
     qDebug() << "ParserAbstract::getJourneyDetails";
 }

 void ParserAbstract::parseJourneyDetails(QNetworkReply *networkReply)
 {
     Q_UNUSED(networkReply);
     qDebug() << "ParserAbstract::parseJourneyDetails";
 }

 QByteArray ParserAbstract::gzipDecompress(QByteArray compressData)
 {
     //decompress GZIP data

     const int buffersize = 16384;
     quint8 buffer[buffersize];

     z_stream cmpr_stream;
     cmpr_stream.next_in = (unsigned char *)compressData.data();
     cmpr_stream.avail_in = compressData.size();

     cmpr_stream.zalloc = Z_NULL;
     cmpr_stream.zfree = Z_NULL;
     cmpr_stream.opaque = Z_NULL;

     // We get data in gzip format, and to parse it, according
     // to the documentation, we need to add 16 to windowBits.
     if (inflateInit2(&cmpr_stream, MAX_WBITS + 16) != Z_OK) {
         qDebug() << "cmpr_stream error!";
     }

     QByteArray uncompressed;
     do {
         cmpr_stream.next_out = buffer;
         cmpr_stream.avail_out = buffersize;

         int status = inflate( &cmpr_stream, Z_SYNC_FLUSH );

         if(status == Z_OK || status == Z_STREAM_END) {
             uncompressed.append(QByteArray::fromRawData((char *)buffer, buffersize - cmpr_stream.avail_out));
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

 #ifdef BUILD_FOR_UBUNTU
 void ParserAbstract::sendHttpRequestCUrl(QUrl url, QByteArray data, const QList<QPair<QByteArray,QByteArray> > &additionalHeaders)
 {
     QStringList headers;

     headers << "Content-Type: application/x-www-form-urlencoded" \
             << QString("User-Agent: %1").arg(userAgent) \
             << "Cache-Control: no-cache";
     for (QList<QPair<QByteArray,QByteArray> >::ConstIterator it = additionalHeaders.constBegin(); it != additionalHeaders.constEnd(); ++it)
         headers << QString("%1: %2").arg(QString::fromLatin1(it->first), QString::fromLatin1(it->second));
     if (!acceptEncoding.isEmpty()) {
         headers << QString("Accept-Encoding: %1").arg(QString::fromLatin1(acceptEncoding));
     }

     QtCUrl::Options opt;
     opt[CURLOPT_URL] = url;
     opt[CURLOPT_HTTPHEADER] = headers;

     if (! data.isNull()) {
       opt[CURLOPT_POST] = true;
       opt[CURLOPT_POSTFIELDS] = data;
     }
     //opt[CURLOPT_FOLLOWLOCATION] = true;
     //opt[CURLOPT_FAILONERROR] = true;

     requestTimeout->start(30000);

     QtCUrl cUrl;
     cUrl.exec(opt);
     QByteArray outputData = cUrl.buffer();

     QCustomNetworkReply * reply = new QCustomNetworkReply();
     reply->setContent(outputData);

     lastRequest = reply;
     networkReplyFinished(lastRequest);
 }
#endif
