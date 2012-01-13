/*******************************************************************************

    This file is a part of Fahrplan for maemo 2009-2011

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

#include "parser_131500comau.h"

Parser131500ComAu::Parser131500ComAu(QObject *parent)
{
     Q_UNUSED(parent);
}

bool Parser131500ComAu::supportsGps()
{
    return false;
}

bool Parser131500ComAu::supportsVia()
{
    return false;
}


QStringList Parser131500ComAu::getTrainRestrictions()
{
    QStringList result;
    result.append(tr("All, except School Buses"));
    result.append(tr("Regular Buses"));
    result.append(tr("Trains"));
    result.append(tr("Ferries"));
    result.append(tr("STA School Bus"));
    return result;
}

void Parser131500ComAu::findStationsByName(QString stationName)
{
    if (currentRequestState != FahrplanNS::noneRequest) {
        return;
    }

    currentRequestState = FahrplanNS::stationsByNameRequest;

    QString fullUrl = "http://www.131500.com.au/plan-your-trip/trip-planner?session=invalidate&itd_cmd=invalid&itd_includedMeans=checkbox&itd_inclMOT_5=1&itd_inclMOT_7=1&itd_inclMOT_1=1&itd_inclMOT_9=1&itd_anyObjFilter_origin=2&itd_anyObjFilter_destination=0&itd_name_destination=Enter+location&x=37&y=12&itd_itdTripDateTimeDepArr=dep&itd_itdTimeHour=-&itd_itdTimeMinute=-&itd_itdTimeAMPM=pm";
    fullUrl.append("&itd_itdDate=" + QDate::currentDate().toString("yyyyMMdd"));
    fullUrl.append("&itd_name_origin=" + stationName);

    sendHttpRequest(QUrl(fullUrl));
}

void Parser131500ComAu::findStationsByCoordinates(qreal longitude, qreal latitude)
{
    Q_UNUSED(longitude);
    Q_UNUSED(latitude);

    emit errorOccured(tr("Coordinates search not supported."));
}

void Parser131500ComAu::parseStationsByName(QNetworkReply *networkReply)
{
    StationsResultList result;

    QRegExp regexp = QRegExp("<select name=\"(.*)\" id=\"from\" size=\"6\" class=\"multiple\">(.*)</select>");
    regexp.setMinimal(true);

    regexp.indexIn(networkReply->readAll());

    QString element = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?><html xmlns=\"http://www.w3.org/1999/xhtml\">\n<body>\n" + regexp.cap(0) + "\n</body>\n</html>\n";

    QBuffer readBuffer;
    readBuffer.setData(element.toAscii());
    readBuffer.open(QIODevice::ReadOnly);

    QXmlQuery query;
    query.bindVariable("path", &readBuffer);
    //Query for more than one result
    query.setQuery("declare default element namespace \"http://www.w3.org/1999/xhtml\"; declare variable $path external; doc($path)/html/body/select/option/string()");

    QStringList stationNames;
    if (!query.evaluateTo(&stationNames))
    {
        qDebug() << "parser131500ComAu::getStationsByName - Query 1 Failed";
    }

    for (int i = 0; i < stationNames.count(); i++) {
        //Remove unneeded stuff from the result
        stationNames[i].replace(" (Location)", "");
        StationsResultItem *item = new StationsResultItem();
        item->setStationName(stationNames[i].trimmed());
        result.appendItem(item);
    }

    emit stationsResult(&result);
}

void Parser131500ComAu::searchJourney(QString departureStation, QString arrivalStation, QString viaStation, QDate date, QTime time, int mode, int trainrestrictions)
{
    if (currentRequestState != FahrplanNS::noneRequest) {
        return;
    }

    currentRequestState = FahrplanNS::searchJourneyRequest;
    Q_UNUSED(viaStation);
    QString modeString = "dep";
    if (mode == 0) {
        modeString = "arr";
    }

    QString hourStr = "am";
    int hour = time.toString("hh").toInt();
    if (hour > 12) {
        hour = hour - 12;
        hourStr = "pm";
    }

    //Request one. (Station selection and receiving an up to date cookie.)
    QString fullUrl = "http://www.131500.com.au/plan-your-trip/trip-planner?session=invalidate&itd_cmd=invalid&itd_includedMeans=checkbox&itd_inclMOT_7=1&itd_anyObjFilter_origin=2&itd_anyObjFilter_destination=2&x=37&y=12";
    fullUrl.append("&itd_itdDate=" + date.toString("yyyyMMdd"));
    fullUrl.append("&itd_itdTimeHour=" + QString::number(hour));
    fullUrl.append("&itd_itdTimeMinute=" + time.toString("mm"));
    fullUrl.append("&itd_itdTripDateTimeDepArr=" + modeString);
    fullUrl.append("&itd_itdTimeAMPM=" + hourStr);
    fullUrl.append("&itd_name_origin=" + departureStation);
    fullUrl.append("&itd_name_destination=" + arrivalStation);

    // itd_inclMOT_5 = bus
    // itd_inclMOT_1 = train
    // itd_inclMOT_9 = ferry
    // itd_inclMOT_11 = school bus
    if (trainrestrictions == 0) {
      fullUrl.append("&itd_inclMOT_5=1&itd_inclMOT_1=1&itd_inclMOT_9=1");
    }
    if (trainrestrictions == 1) {
      fullUrl.append("&itd_inclMOT_5=1");
    }
    if (trainrestrictions == 2) {
      fullUrl.append("&itd_inclMOT_1=1");
    }
    if (trainrestrictions == 3) {
      fullUrl.append("&itd_inclMOT_9=1");
    }
    if (trainrestrictions == 4) {
      fullUrl.append("&itd_inclMOT_11=1");
    }

    //qDebug()<<fullUrl;
    sendHttpRequest(QUrl(fullUrl));
}

void Parser131500ComAu::parseSearchJourney(QNetworkReply *networkReply)
{
    lastJourneyResultList = new JourneyResultList();

    QBuffer *filebuffer = new QBuffer();
    filebuffer->setData(networkReply->readAll());

    QRegExp regexp = QRegExp("<div class=\"midcolumn3\">(.*)</div>(.*)</div>(.*)<div id=\"righttools\">");
    regexp.setMinimal(true);

    regexp.indexIn(filebuffer->buffer());

    QString element = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?><html xmlns=\"http://www.w3.org/1999/xhtml\">\n<body>\n" + regexp.cap(0) + "\n</div></body>\n</html>\n";

    QRegExp imgReg = QRegExp("icon_(.*)_s.gif\" />");
    imgReg.setMinimal(true);
    element.replace(imgReg, "icon_" + QString("\\1") + "_s.gif\" />" + QString("\\1"));

    //qDebug()<<element;

    QBuffer readBuffer;
    readBuffer.setData(element.toAscii());
    readBuffer.open(QIODevice::ReadOnly);

    QXmlQuery query;
    query.bindVariable("path", &readBuffer);
    query.setQuery("declare default element namespace \"http://www.w3.org/1999/xhtml\"; declare variable $path external; doc($path)/html/body/div/div/table/tbody/tr/td[@id='header2']/string()");

    QStringList departResult;
    if (!query.evaluateTo(&departResult))
    {
        qDebug() << "parser131500ComAu::getJourneyData - Query 1 Failed";
    }

    query.setQuery("declare default element namespace \"http://www.w3.org/1999/xhtml\"; declare variable $path external; doc($path)/html/body/div/div/table/tbody/tr/td[@id='header3']/string()");

    QStringList arriveResult;
    if (!query.evaluateTo(&arriveResult))
    {
        qDebug() << "parser131500ComAu::getJourneyData - Query 2 Failed";
    }

    query.setQuery("declare default element namespace \"http://www.w3.org/1999/xhtml\"; declare variable $path external; doc($path)/html/body/div/div/table/tbody/tr/td[@id='header4']/string()");

    QStringList timeResult;
    if (!query.evaluateTo(&timeResult))
    {
        qDebug() << "parser131500ComAu::getJourneyData - Query 3 Failed";
    }

    query.setQuery("declare default element namespace \"http://www.w3.org/1999/xhtml\"; declare variable $path external; doc($path)/html/body/div/div/table/tbody/tr/td[@id='header5']/string()");

    QStringList trainResult;
    if (!query.evaluateTo(&trainResult))
    {
        qDebug() << "parser131500ComAu::getJourneyData - Query 4 Failed";
    }

    query.setQuery("declare default element namespace \"http://www.w3.org/1999/xhtml\"; declare variable $path external; doc($path)/html/body/div/div/div/string()");

    QStringList headerResult;
    if (!query.evaluateTo(&headerResult))
    {
        qDebug() << "parser131500ComAu::getJourneyData - Query 5 Failed";
    }

    for (int i = 0; i < headerResult.count(); i++) {
        QRegExp regexp = QRegExp("(From:|To:|When:)(.*)$");
        regexp.setMinimal(true);
        regexp.indexIn(headerResult[i].trimmed());
        if (regexp.cap(1) == "From:") {
            lastJourneyResultList->setDepartureStation(regexp.cap(2).trimmed());
        }
        if (regexp.cap(1) == "To:") {
            lastJourneyResultList->setArrivalStation(regexp.cap(2).trimmed());
        }
        if (regexp.cap(1) == "When:") {
            lastJourneyResultList->setTimeInfo(regexp.cap(2).trimmed());
        }
    }

    QRegExp regexp2 = QRegExp("(.*), (\\d\\d) (.*) (\\d\\d\\d\\d)");
    regexp2.setMinimal(true);
    regexp2.indexIn(lastJourneyResultList->timeInfo().trimmed());
    QLocale enLocale = QLocale(QLocale::English, QLocale::UnitedStates);
    int month = 1;
    for (month = 1; month < 10; month++) {
        if (regexp2.cap(3).trimmed() == enLocale.standaloneMonthName(month)) {
            break;
        }
    }

    QDate journeydate = QDate::fromString(regexp2.cap(2).trimmed() + " " + QString::number(month) + " " + regexp2.cap(4).trimmed(), "dd M yyyy");

    //Generate Details search results

    QStringList detailsResult;

    regexp = QRegExp("<table class=\"dataTbl widthcol2and3\" cellspacing=\"0\" style=\"margin:0px ! important;border-right:0px none;\" summary=\"Search Results Details\">(.*)</table>");
    regexp.setMinimal(true);
    int pos = 0;

    while ((pos = regexp.indexIn(filebuffer->buffer(), pos)) != -1) {
        QString element = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?><html xmlns=\"http://www.w3.org/1999/xhtml\">\n<body><table>\n" + regexp.cap(1) + "\n</table></body>\n</html>\n";
        element.replace("&nbsp;", " ");
        element.replace("bulletin.gif\">", "bulletin.gif\" />");

        QBuffer readBuffer;
        readBuffer.setData(element.toAscii());
        readBuffer.open(QIODevice::ReadOnly);

        QXmlQuery query;
        query.bindVariable("path", &readBuffer);

        query.setQuery("declare default element namespace \"http://www.w3.org/1999/xhtml\"; declare variable $path external; doc($path)/html/body/table/tbody/tr/td[@headers='header2']/string()");

        QStringList detailsContentResult;
        if (!query.evaluateTo(&detailsContentResult))
        {
            qDebug() << "parser131500ComAu::getJourneyData - DetailsQuery 1 Failed";
        }

        detailsResult << detailsContentResult.join("<linesep>");

        pos += regexp.matchedLength();
    }

    //Create search result
    for (int i = 0; i < departResult.count(); i++) {

        //Parse transporttypes and changes
        QString tmp = trainResult[i].trimmed();
        tmp.replace("\t", " ");
        tmp.replace("\n", " ");
        tmp.replace("\r", " ");
        QStringList trains = tmp.split(" ", QString::SkipEmptyParts);
        int changes = trains.length() - 1;
        trains.removeDuplicates();

        //Parse travel time
        tmp = timeResult[i].trimmed();
        tmp.replace("mins", "");
        tmp.replace("min", "");
        tmp.replace("hrs ", ":");
        tmp.replace("hr ", ":");
        QStringList durationLst = tmp.split(":", QString::SkipEmptyParts);

        QString durationStr = "";
        if (durationLst.length() == 1) {
            durationStr.sprintf("00: %02d", durationLst[0].toInt());
        }
        if (durationLst.length() == 2) {
            durationStr.sprintf("%02d:%02d", durationLst[0].toInt(), durationLst[1].toInt());
        }

        JourneyResultItem *item = new JourneyResultItem();
        item->setDate(journeydate);
        item->setId(QString::number(i));
        item->setTransfers(QString::number(changes));
        item->setDuration(durationStr);
        item->setTrainType(trains.join(", "));
        item->setDepartureTime(QTime::fromString(departResult[i].trimmed(), "h:map").toString("hh:mm"));
        item->setArrivalTime(QTime::fromString(arriveResult[i].trimmed(), "h:map").toString("hh:mm"));
        item->setInternalData1(detailsResult[i]);

        lastJourneyResultList->appendItem(item);
    }

    emit journeyResult(lastJourneyResultList);
}

void Parser131500ComAu::getJourneyDetails(QString id)
{
    if (currentRequestState != FahrplanNS::noneRequest) {
        return;
    }

    //Some hafasxml backend provide the detailsdata inline
    //if so our parser already stored them
    if (lastJourneyResultList->itemcount() > 0 ) {

        for (int i = 0; i < lastJourneyResultList->itemcount(); i++) {
            JourneyResultItem *item = lastJourneyResultList->getItem(i);
            if (item->id() == id) {
                emit journeyDetailsResult(parseDetails(item));
                return;
            }
        }
    }

    emit errorOccured(tr("Internal error occured: JourneyResultdata not present!"));
    return;
}

JourneyDetailResultList * Parser131500ComAu::parseDetails(JourneyResultItem *journeyitem)
{
    QStringList detailResults = journeyitem->internalData1().split("<linesep>");
    JourneyDetailResultList *results = new JourneyDetailResultList();

    QDate journeydate = journeyitem->date();

    for (int i = 0; i < detailResults.count(); i++) {
        JourneyDetailResultItem *item = new JourneyDetailResultItem();

        QRegExp regexp = QRegExp("(Take the |Walk to )(.*)$");
        regexp.setMinimal(true);
        regexp.indexIn(detailResults[i].trimmed());

        if (regexp.cap(1) == "Take the ") {
            qDebug()<<"Regular: "<<regexp.cap(2).trimmed();
            QRegExp regexp2 = QRegExp("(.*)Dep: (\\d:\\d\\d|\\d\\d:\\d\\d)(am|pm) (.*)Arr: (\\d:\\d\\d|\\d\\d:\\d\\d)(am|pm) (.*)(\\t+.*)$");
            regexp2.setMinimal(true);
            regexp2.indexIn(regexp.cap(2).trimmed());
            qDebug()<<"***";
            if (regexp2.matchedLength() == -1) {
                regexp2 = QRegExp("(.*)Dep: (\\d:\\d\\d|\\d\\d:\\d\\d)(am|pm) (.*)Arr: (\\d:\\d\\d|\\d\\d:\\d\\d)(am|pm) (.*)$");
                regexp2.setMinimal(true);
                regexp2.indexIn(regexp.cap(2).trimmed());
            }

            /*
            qDebug()<<"Train:"<<regexp2.cap(1).trimmed();
            qDebug()<<"Time1:"<<regexp2.cap(2).trimmed();
            qDebug()<<"Time1b:"<<regexp2.cap(3).trimmed();
            qDebug()<<"Station1:"<<regexp2.cap(4).trimmed();
            qDebug()<<"Time2:"<<regexp2.cap(5).trimmed();
            qDebug()<<"Time2b:"<<regexp2.cap(6).trimmed();
            qDebug()<<"Station2:"<<regexp2.cap(7).trimmed();
            qDebug()<<"Alt:"<<regexp2.cap(8).trimmed();
            */

            QDateTime fromDateTime;
            QDateTime toDateTime;
            QTime fromTime   = QTime::fromString(regexp2.cap(2).trimmed() + regexp2.cap(3).trimmed(), "h:map");
            QTime toTime     = QTime::fromString(regexp2.cap(5).trimmed() + regexp2.cap(6).trimmed(), "h:map");
            fromDateTime.setDate(journeydate);
            fromDateTime.setTime(fromTime);
            toDateTime.setDate(journeydate);
            toDateTime.setTime(toTime);

            if (toDateTime.toTime_t() < fromDateTime.toTime_t()) {
                toDateTime.addDays(1);
                journeydate.addDays(1);
            }

            item->setDepartureStation(regexp2.cap(4).trimmed());
            item->setArrivalStation(regexp2.cap(7).trimmed());
            item->setTrain(regexp2.cap(1).trimmed());
            item->setInfo(regexp2.cap(8).trimmed());
            item->setDepartureDateTime(fromDateTime);
            item->setArrivalDateTime(toDateTime);
            results->appendItem(item);
        }
        if (regexp.cap(1) == "Walk to ") {
            //qDebug()<<"Walking: "<<regexp.cap(2).trimmed();
            QRegExp regexp2 = QRegExp("(.*) - (.+) (.*)$");
            regexp2.setMinimal(true);
            regexp2.indexIn(regexp.cap(2).trimmed());

            /*
            qDebug()<<"***";
            qDebug()<<"Station1:"<<regexp2.cap(1).trimmed();
            qDebug()<<"WalkDist1:"<<regexp2.cap(2).trimmed();
            qDebug()<<"WalkDist2:"<<regexp2.cap(3).trimmed();
            */

            item->setDepartureStation("");
            if (results->itemcount() > 0) {
                JourneyDetailResultItem *lastitem = results->getItem(results->itemcount() - 1);
                item->setDepartureStation(lastitem->arrivalStation());
                item->setDepartureDateTime(lastitem->arrivalDateTime());
                item->setArrivalDateTime(lastitem->arrivalDateTime());
            }
            item->setArrivalStation(regexp2.cap(1).trimmed());
            item->setInfo("Walking " + regexp2.cap(2).trimmed() + " " + regexp2.cap(3).trimmed());
            //Don't add WalkTo infos as first item
            if (results->itemcount() > 0) {
                results->appendItem(item);
            }

        }
    }

    results->setDuration(journeyitem->duration());
    if (results->itemcount() > 0) {
        JourneyDetailResultItem *lastitem = results->getItem(results->itemcount() - 1);
        JourneyDetailResultItem *firstitem = results->getItem(0);
        results->setDepartureStation(firstitem->departureStation());
        results->setArrivalStation(lastitem->arrivalStation());

        for (int i=0; i < results->itemcount(); i++) {
            if (!results->getItem(i)->departureDateTime().isNull()) {
                results->setDepartureDateTime(results->getItem(i)->departureDateTime());
                break;
            }
        }

        for (int i=results->itemcount() -1; i >= 0; i--) {
            if (!results->getItem(i)->arrivalDateTime().isNull()) {
                results->setArrivalDateTime(results->getItem(i)->arrivalDateTime());
                break;
            }
        }
    }

    return results;
}
