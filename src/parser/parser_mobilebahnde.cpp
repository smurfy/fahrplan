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

#include "parser_mobilebahnde.h"

ParserMobileBahnDe::ParserMobileBahnDe(QObject *parent)
{
     Q_UNUSED(parent);
    //http://reiseauskunft.bahn.de/bin/extxml.exe
     baseXmlUrl = "http://mobile.bahn.de/bin/mobil/query.exe";
     baseSTTableUrl = "http://mobile.bahn.de/bin/mobil/stboard.exe/en";
     baseUrl = "http://mobile.bahn.de/bin/mobil/query.exe";
     STTableMode = 1;
}

bool ParserMobileBahnDe::supportsTimeTable()
{
    return true;
}

bool ParserMobileBahnDe::supportsGps()
{
    return true;
}

bool ParserMobileBahnDe::supportsVia()
{
    return false;
}

QStringList ParserMobileBahnDe::getTrainRestrictions()
{
    QStringList result;
    result.append(tr("All"));
    result.append(tr("All without ICE"));
    result.append(tr("Only local transport"));
    result.append(tr("Local transport without S-Bahn"));
    return result;
}

QString ParserMobileBahnDe::getTrainRestrictionsCodes(int trainrestrictions)
{
    QString trainrestr = "1:1111111111000000";
    if (trainrestrictions == 0) {
        trainrestr = "1:1111111111000000"; //ALL
    } else if (trainrestrictions == 1) {
        trainrestr = "2:0111111111000000"; //All without ICE
    } else if (trainrestrictions == 2) {
        trainrestr = "4:00011111110000000"; //Only local transport
    } else if (trainrestrictions == 3) {
        trainrestr = "4:0001011111000000"; //Only local transport without S-Bahn
    }

    return trainrestr;
}

 void ParserMobileBahnDe::searchJourney(QString departureStation, QString arrivalStation, QString viaStation, QDate date, QTime time, int mode, int trainrestrictions)
 {
     if (currentRequestState != FahrplanNS::noneRequest) {
         return;
     }

     currentRequestState = FahrplanNS::searchJourneyRequest;

     searchJourneyRequestData.progress = 1;
     searchJourneyRequestData.departureStation = departureStation;
     searchJourneyRequestData.arrivalStation = arrivalStation;
     searchJourneyRequestData.viaStation = viaStation;
     searchJourneyRequestData.date = date;
     searchJourneyRequestData.time = time;
     searchJourneyRequestData.trainrestrictions = trainrestrictions;

     QString trainrestr = getTrainRestrictionsCodes(trainrestrictions);

     //Postdata for normal Search
     QString postData = "REQ0JourneyStopsS0A=1&REQ0JourneyStopsS0G=" +
                           departureStation +
                           "&REQ0JourneyStopsS0ID=&REQ0JourneyStopsZ0A=1&REQ0JourneyStopsZ0G=" +
                           arrivalStation +
                           "&REQ0JourneyStopsZ0ID=&REQ0JourneyDate=" +
                           date.toString("dd.MM.yyyy") +
                           "&REQ0JourneyTime=" +
                           time.toString("hh:mm") +
                           "&REQ0HafasSearchForw=" +
                           QString::number(mode) +
                           "&REQ0JourneyProduct_prod_list=" +
                           trainrestr +
                           "&existOptimizePrice=1&REQ0HafasOptimize1=0%3A1&existProductNahverkehr=yes&REQ0Tariff_TravellerAge.1=35&start=Suchen&REQ0Tariff_Class=2&REQ0Tariff_TravellerReductionClass.1=0";

     sendHttpRequest(QUrl("http://mobile.bahn.de/bin/mobil/query.exe/eox?rt=1&use_realtime_filter=1&searchMode=NORMAL"), postData.toAscii());
 }

 void ParserMobileBahnDe::parseSearchJourney(QNetworkReply *networkReply)
 {
     switch (searchJourneyRequestData.progress) {
     case 1:
         parseSearchJourneyPart1(networkReply->readAll());
         break;
     case 2:
         parseSearchJourneyPart2(networkReply->readAll());
         break;
     }
 }

 void ParserMobileBahnDe::parseSearchJourneyPart1(QString data)
 {
     //Remove misformed html
     QRegExp ahrefReg = QRegExp("class=\"arrowlink\" href=\"(.*)\"");
     ahrefReg.setMinimal(true);
     data.replace(ahrefReg, "");

     //Check if we need do a search by id, sometimes the search for the exact name
     //returns a selectbox again, so we check for it and if there are selectboxes
     //we search by the ids we get as response

     QBuffer readBuffer;
     readBuffer.setData(data.toAscii());
     readBuffer.open(QIODevice::ReadOnly);

     QXmlQuery query;
     query.bindVariable("path", &readBuffer);
     //Query for more than one result
     query.setQuery("declare default element namespace \"http://www.w3.org/1999/xhtml\"; declare variable $path external; doc($path)/html/body/div/div/div/form/div/select[@name='REQ0JourneyStopsS0K']/option/string()");

     QStringList departureResult;
     if (!query.evaluateTo(&departureResult))
     {
         qDebug() << "parserMobileBahnDe::getJourneyData - Query 1 Failed";
     }

     query.setQuery("declare default element namespace \"http://www.w3.org/1999/xhtml\"; declare variable $path external; doc($path)/html/body/div/div/div/form/div/select[@name='REQ0JourneyStopsZ0K']/option/string()");

     QStringList arrivalResult;
     if (!query.evaluateTo(&arrivalResult))
     {
         qDebug() << "parserMobileBahnDe::getJourneyData - Query 2 Failed";
     }

     query.setQuery("declare default element namespace \"http://www.w3.org/1999/xhtml\"; declare variable $path external; doc($path)/html/body/div/div/div/form/div/select[@name='REQ0JourneyStopsS0K']/option/@value/string()");

     QStringList departureResultIds;
     if (!query.evaluateTo(&departureResultIds))
     {
         qDebug() << "parserMobileBahnDe::getJourneyData - Query 3 Failed";
     }

     query.setQuery("declare default element namespace \"http://www.w3.org/1999/xhtml\"; declare variable $path external; doc($path)/html/body/div/div/div/form/div/select[@name='REQ0JourneyStopsZ0K']/option/@value/string()");

     QStringList arrivalResultIds;
     if (!query.evaluateTo(&arrivalResultIds))
     {
         qDebug() << "parserMobileBahnDe::getJourneyData - Query 4 Failed";
     }

     query.setQuery("declare default element namespace \"http://www.w3.org/1999/xhtml\"; declare variable $path external; doc($path)/html/body/div/div/div/form/@action/string()");

     QStringList formUrl;
     if (!query.evaluateTo(&formUrl))
     {
         qDebug() << "parserMobileBahnDe::getJourneyData - Query 5 Failed";
     }

     query.setQuery("declare default element namespace \"http://www.w3.org/1999/xhtml\"; declare variable $path external; doc($path)/html/body/div/div/div/form/div/input[@name='REQ0JourneyStopsS0K']/@value/string()");

     QStringList departureStationIds;
     if (!query.evaluateTo(&departureStationIds))
     {
         qDebug() << "parserMobileBahnDe::getJourneyData - Query 6 Failed";
     }

     query.setQuery("declare default element namespace \"http://www.w3.org/1999/xhtml\"; declare variable $path external; doc($path)/html/body/div/div/div/form/div/input[@name='REQ0JourneyStopsZ0K']/@value/string()");

     QStringList arrivalStationIds;
     if (!query.evaluateTo(&arrivalStationIds))
     {
         qDebug() << "parserMobileBahnDe::getJourneyData - Query 7 Failed";
     }

     if (departureResult.count() > 0 || arrivalResult.count() > 0)
     {
         QString departureStationId = "";
         QString arrivalStationId = "";

         if (departureResult.count() > 0)
         {
             for (int i = 0; i < departureResult.count(); i++)
             {
                 if ((QString)departureResult[i].trimmed() == searchJourneyRequestData.departureStation.trimmed())
                 {
                     departureStationId = departureResultIds[i];
                 }
             }
         } else
         {
             departureStationId = departureStationIds[0];
         }

         if (arrivalResult.count() > 0)
         {
             for (int i = 0; i < arrivalResult.count(); i++)
             {
                 if ((QString)arrivalResult[i].trimmed() == searchJourneyRequestData.arrivalStation.trimmed())
                 {
                     arrivalStationId = arrivalResultIds[i];
                 }
             }
         } else
         {
             arrivalStationId = arrivalStationIds[0];
         }

         searchJourneyRequestData.progress = 2;
         currentRequestState = FahrplanNS::searchJourneyRequest;

         QString trainrestr = getTrainRestrictionsCodes(searchJourneyRequestData.trainrestrictions);

         QString postData = "REQ0JourneyStopsS0A=1&REQ0JourneyStopsS0K=" +
                                   departureStationId +
                                   "&REQ0JourneyStopsS0ID=&REQ0JourneyStopsZ0A=1&REQ0JourneyStopsZ0K=" +
                                   arrivalStationId +
                                   "&REQ0JourneyStopsZ0ID=&REQ0JourneyDate=" +
                                   searchJourneyRequestData.date.toString("dd.MM.yyyy") +
                                   "&REQ0JourneyTime=" +
                                   searchJourneyRequestData.time.toString("hh:mm") +
                                   "&REQ0HafasSearchForw=" +
                                   QString::number(searchJourneyRequestData.mode) +
                                   "&REQ0JourneyProduct_prod_list=" +
                                   trainrestr +
                                   "&existOptimizePrice=1&REQ0HafasOptimize1=0%3A1&existProductNahverkehr=yes&REQ0Tariff_TravellerAge.1=35&start=Suchen&REQ0Tariff_Class=2&REQ0Tariff_TravellerReductionClass.1=0";

         sendHttpRequest(QUrl(formUrl[0].trimmed()), postData.toAscii());
     } else {
         parseSearchJourneyPart2(data);
     }
 }

 void ParserMobileBahnDe::parseSearchJourneyPart2(QString data)
{
     //Remove misformed html
     QRegExp ahrefReg = QRegExp("class=\"arrowlink\" href=\"(.*)\"");
     ahrefReg.setMinimal(true);
     data.replace(ahrefReg, "");

     data.replace("<img src=\"/v/760/img/achtung_rot_16x16.gif\" />", "<img src=\"/v/760/img/achtung_rot_16x16.gif\" />" + tr("Warning"));

     QBuffer readBuffer;
     readBuffer.setData(data.toAscii());
     readBuffer.open(QIODevice::ReadOnly);

     QXmlQuery query;
     query.bindVariable("path", &readBuffer);
     //Query for Header infos
     query.setQuery("declare default element namespace \"http://www.w3.org/1999/xhtml\"; declare variable $path external; doc($path)/html/body/div/div/div/div[@class='haupt']/span[@class='bold']/string()");

     QStringList headerResults;
     if (!query.evaluateTo(&headerResults))
     {
         qDebug() << "parserMobileBahnDe::parseJourneyData - Query 1 Failed";
     }

     //Query for Time infos
     query.setQuery("declare default element namespace \"http://www.w3.org/1999/xhtml\"; declare variable $path external; doc($path)/html/body/div/div/table/tr/td[@class='overview timelink']/descendant::text()/string()");

     QStringList timelinkResults;
     if (!query.evaluateTo(&timelinkResults))
     {
         qDebug() << "parserMobileBahnDe::parseJourneyData - Query 2 Failed";
     }

     //Query for state infos
     query.setQuery("declare default element namespace \"http://www.w3.org/1999/xhtml\"; declare variable $path external; doc($path)/html/body/div/div/table/tr/td[@class='overview tprt']/string()");

     QStringList stateResults;
     if (!query.evaluateTo(&stateResults))
     {
         qDebug() << "parserMobileBahnDe::parseJourneyData - Query 3 Failed";
     }

     //Query for chanes infos
     query.setQuery("declare default element namespace \"http://www.w3.org/1999/xhtml\"; declare variable $path external; doc($path)/html/body/div/div/table/tr/td[@class='overview']/descendant::text()/string()");

     QStringList changesResults;
     if (!query.evaluateTo(&changesResults))
     {
         qDebug() << "parserMobileBahnDe::parseJourneyData - Query 4 Failed";
     }

     //Query for train infos
     query.setQuery("declare default element namespace \"http://www.w3.org/1999/xhtml\"; declare variable $path external; doc($path)/html/body/div/div/table/tr/td[@class='overview iphonepfeil']/descendant::text()[1]/string()");

     QStringList trainResults;
     if (!query.evaluateTo(&trainResults))
     {
         qDebug() << "parserMobileBahnDe::parseJourneyData - Query 5 Failed";
     }

     //Query for error infos
     query.setQuery("declare default element namespace \"http://www.w3.org/1999/xhtml\"; declare variable $path external; doc($path)/html/body/div/div/div/form/div[@class='fline errormsg']/string()");

     QStringList errorsResults;
     if (!query.evaluateTo(&errorsResults))
     {
         qDebug() << "parserMobileBahnDe::parseJourneyData - Query 6 Failed";
     }

     //Query for later url
     query.setQuery("declare default element namespace \"http://www.w3.org/1999/xhtml\"; declare variable $path external; doc($path)/html/body/div/div/ul[@class='neben']/li/a[@class='noBG']/@href/string()");

     QStringList laterUrl;
     if (!query.evaluateTo(&laterUrl))
     {
         qDebug() << "parserMobileBahnDe::parseJourneyData - Query 7 Failed";
     }

     //Query for Details Link infos
     query.setQuery("declare default element namespace \"http://www.w3.org/1999/xhtml\"; declare variable $path external; doc($path)/html/body/div/div/table/tr/td[@class='overview timelink']/a/@href/string()");

     QStringList detailslinkResults;
     if (!query.evaluateTo(&detailslinkResults))
     {
         qDebug() << "parserMobileBahnDe::parseJourneyData - Query 8 Failed";
     }

     if (errorsResults.count() > 0)
     {
         emit errorOccured(errorsResults[0].trimmed());
         return;
     }

     lastJourneyResultList = new JourneyResultList();

     if (headerResults.count() < 3)
     {
         emit journeyResult(lastJourneyResultList);
         return;
     }

     if (laterUrl.count() == 1)
     {
         lastLaterUrl = laterUrl[0];
         lastEarlierUrl = laterUrl[0];
         lastEarlierUrl.replace("e=1", "e=2");
     }

     QDate date = QDate::fromString(headerResults[2].trimmed().right(8), "dd.MM.yy").addYears(100);

     for(int i=0; i < stateResults.count(); i++)
     {
         JourneyResultItem *item = new JourneyResultItem();
         item->setDate(date);
         item->setTransfers(changesResults[i * 2].trimmed());
         item->setDuration(changesResults[(i * 2) + 1].trimmed());
         item->setMiscInfo(stateResults[i].trimmed());
         item->setTrainType(trainResults[i].trimmed());
         item->setId(QString::number(i));
         item->setDepartureTime(timelinkResults[i * 2].trimmed());
         item->setArrivalTime(timelinkResults[(i * 2) + 1].trimmed());
         item->setInternalData1(detailslinkResults[i].trimmed());
         lastJourneyResultList->appendItem(item);
     }

     lastJourneyResultList->setDepartureStation(headerResults[0].trimmed());
     lastJourneyResultList->setArrivalStation(headerResults[1].trimmed());
     lastJourneyResultList->setTimeInfo(date.toString());

     emit journeyResult(lastJourneyResultList);
 }

 void ParserMobileBahnDe::searchJourneyLater()
 {
     if (lastLaterUrl.isEmpty()) {
         emit errorOccured(tr("Internal error occured, going later is not possible"));
         return;
     }

     if (currentRequestState != FahrplanNS::noneRequest) {
         return;
     }

     currentRequestState = FahrplanNS::searchJourneyLaterRequest;

     sendHttpRequest(QUrl(lastLaterUrl));
 }

 void ParserMobileBahnDe::searchJourneyEarlier()
 {
     if (lastEarlierUrl.isEmpty()) {
         emit errorOccured(tr("Internal error occured, going earlier is not possible"));
         return;
     }

     if (currentRequestState != FahrplanNS::noneRequest) {
         return;
     }

     currentRequestState = FahrplanNS::searchJourneyEarlierRequest;

     sendHttpRequest(QUrl(lastEarlierUrl));
 }

 void ParserMobileBahnDe::parseSearchLaterJourney(QNetworkReply *networkReply)
 {
     parseSearchJourneyPart2(networkReply->readAll());
 }

 void ParserMobileBahnDe::parseSearchEalierJourney(QNetworkReply *networkReply)
 {
    parseSearchJourneyPart2(networkReply->readAll());
 }

 void ParserMobileBahnDe::getJourneyDetails(QString id)
 {
     if (currentRequestState != FahrplanNS::noneRequest) {
         return;
     }

     int num = id.toInt();
     if (num > lastJourneyResultList->itemcount()) {
        emit errorOccured(tr("Internal error occured, requesting invalid details data."));
        return;
     }

     currentRequestState = FahrplanNS::journeyDetailsRequest;

     journeyDetailRequestData.date = lastJourneyResultList->getItem(num)->date();
     journeyDetailRequestData.duration = lastJourneyResultList->getItem(num)->duration();
     journeyDetailRequestData.id = id;

     QString url = lastJourneyResultList->getItem(num)->internalData1();
     sendHttpRequest(QUrl(url));
 }

 void ParserMobileBahnDe::parseJourneyDetails(QNetworkReply *networkReply)
 {
     JourneyDetailResultList *results = new JourneyDetailResultList();
     QString xhtml = networkReply->readAll();

     //We use Regex to get each element details page
     QRegExp regexp = QRegExp("<div class=\"rline haupt\".*>(.*)</div>");
     regexp.setMinimal(true);

     int idx = 0;
     int num = 0;
     while (idx >= 0)
     {
         idx = regexp.indexIn(xhtml, idx);
         if (idx >= 0)
         {
             num++;
             QBuffer readBuffer;
             QString element = "<html xmlns=\"http://www.w3.org/1999/xhtml\">\n<body>\n" + regexp.cap(0) + "\n</body>\n</html>\n";

             //Remove misformed html
             QRegExp ahrefReg = QRegExp("class=\"arrowlink\" href=\"(.*)\"");
             ahrefReg.setMinimal(true);
             element.replace(ahrefReg, "");
             element.replace("&nbsp;", " ");

             readBuffer.setData(element.toAscii());
             readBuffer.open(QIODevice::ReadOnly);

             QXmlQuery query;
             query.bindVariable("path", &readBuffer);
             query.setQuery("declare default element namespace \"http://www.w3.org/1999/xhtml\"; declare variable $path external; doc($path)/html/body/div/span[@class='bold']/string()");

             QStringList stationResults;
             if (!query.evaluateTo(&stationResults))
             {
                 qDebug() << "parserMobileBahnDe::getJourneyDetailsData - Query 1 Failed";
             }

             query.setQuery("declare default element namespace \"http://www.w3.org/1999/xhtml\"; declare variable $path external; doc($path)/html/body/div/a/span/string()");

             QStringList trainnrResults;
             if (!query.evaluateTo(&trainnrResults))
             {
                 qDebug() << "parserMobileBahnDe::getJourneyDetailsData - Query 2 Failed";
             }

             query.setQuery("declare default element namespace \"http://www.w3.org/1999/xhtml\"; declare variable $path external; doc($path)/html/body/div/child::text()/string()");

             QStringList txtResults;
             if (!query.evaluateTo(&txtResults))
             {
                 qDebug() << "parserMobileBahnDe::getJourneyDetailsData - Query 3 Failed";
             }

             /*
             qDebug() <<num;
             qDebug() <<element;
             qDebug() <<stationResults;
             qDebug() <<trainnrResults;
             qDebug() <<txtResults;
             */

             if (stationResults.count() == 2)
             {
                 JourneyDetailResultItem *item = new JourneyDetailResultItem();

                 item->setDepartureStation(stationResults[0].trimmed());
                 item->setArrivalStation(stationResults[1].trimmed());
                 if (trainnrResults.count() > 0)
                 {
                     item->setTrain(trainnrResults[0].simplified().trimmed());
                 }

                 QString tmp = txtResults.join(" ");
                 tmp.replace("\n", " ");
                 tmp = tmp.simplified();

                 //Check and Parse departure and arrival Times
                 QRegExp tmpRegexp = QRegExp("(dep) (.+) on (.*) (arr) (.+) on (.*)$");
                 tmpRegexp.setMinimal(true);
                 tmpRegexp.indexIn(tmp);

                 if (tmpRegexp.cap(0) != "")
                 {
                     if (tmpRegexp.cap(1) == "dep")
                     {
                         QRegExp tmpRegexp2 = QRegExp("(\\d\\d:\\d\\d)");
                         tmpRegexp2.setMinimal(true);
                         tmpRegexp2.indexIn(tmpRegexp.cap(2).trimmed());

                         QRegExp tmpRegexp3 = QRegExp("(\\d\\d\.\\d\\d\.\\d\\d)");
                         tmpRegexp3.setMinimal(true);
                         tmpRegexp3.indexIn(tmpRegexp.cap(3).trimmed());

                         QDate fromDate = QDate::fromString(tmpRegexp3.cap(1).trimmed(), "dd.MM.yy");
                         QTime fromTime = QTime::fromString(tmpRegexp2.cap(1).trimmed(), "hh:mm");

                         fromDate = fromDate.addYears(100); //default is 1900 so we add 100years

                         QDateTime fromDateTime;
                         fromDateTime.setDate(fromDate);
                         fromDateTime.setTime(fromTime);
                         item->setDepartureDateTime(fromDateTime);

                         item->setDepartureInfo(tmpRegexp.cap(2).mid(5).trimmed());
                     }
                     if (tmpRegexp.cap(4) == "arr")
                     {
                         QRegExp tmpRegexp2 = QRegExp("(\\d\\d:\\d\\d)");
                         tmpRegexp2.setMinimal(true);
                         tmpRegexp2.indexIn(tmpRegexp.cap(5).trimmed());

                         QRegExp tmpRegexp3 = QRegExp("(\\d\\d\.\\d\\d\.\\d\\d)");
                         tmpRegexp3.setMinimal(true);
                         tmpRegexp3.indexIn(tmpRegexp.cap(6).trimmed());

                         QDate toDate = QDate::fromString(tmpRegexp3.cap(1).trimmed(), "dd.MM.yy");
                         QTime toTime = QTime::fromString(tmpRegexp2.cap(1).trimmed(), "hh:mm");

                         toDate = toDate.addYears(100); //default is 1900 so we add 100years

                         QDateTime toDateTime;
                         toDateTime.setDate(toDate);
                         toDateTime.setTime(toTime);
                         item->setArrivalDateTime(toDateTime);

                         item->setArrivalInfo(tmpRegexp.cap(5).mid(5).trimmed());
                     }
                 } else
                 {
                     QString info = item->info();
                     info.append(tmp.trimmed());
                     item->setInfo(info);
                 }

                 results->appendItem(item);
             } else
             {
                 for (int i = 0; i < txtResults.count(); i++)
                 {
                     QString tmp = txtResults[i];
                     tmp.replace("\n", " ");
                     tmp = tmp.simplified();

                     //Check and Parse departure and arrival Times
                     QRegExp tmpRegexp = QRegExp("(Duration|Hint)(.*)$");
                     tmpRegexp.setMinimal(true);
                     tmpRegexp.indexIn(tmp);
                     if (tmpRegexp.cap(0) != "")
                     {
                         if (tmpRegexp.cap(1) == "Duration")
                         {
                             results->setDuration(tmpRegexp.cap(2).trimmed());
                         }
                         if (tmpRegexp.cap(1) == "Hint")
                         {
                             results->setInfo(tmpRegexp.cap(2).trimmed());
                         }
                     }
                 }
             }
             ++idx;
         }
     }

     regexp = QRegExp("<div class=\"red bold\">(.*)</div>");
     regexp.setMinimal(true);
     regexp.indexIn(xhtml);

     if (regexp.cap(1).trimmed() != "") {
         QBuffer readBuffer;
         QString element = "<html xmlns=\"http://www.w3.org/1999/xhtml\">\n<body>\n" + regexp.cap(1) + "\n</body>\n</html>\n";

         //Remove misformed html
         QRegExp ahrefReg = QRegExp("<a(.*)>");
         ahrefReg.setMinimal(true);
         element.replace(ahrefReg, "");
         element.replace("&nbsp;", " ");
         ahrefReg = QRegExp("</a>");
         ahrefReg.setMinimal(true);
         element.replace(ahrefReg, "");
         element.replace("&nbsp;", " ");

         readBuffer.setData(element.toAscii());
         readBuffer.open(QIODevice::ReadOnly);

         QXmlQuery query;
         query.bindVariable("path", &readBuffer);
         query.setQuery("declare default element namespace \"http://www.w3.org/1999/xhtml\"; declare variable $path external; doc($path)/html/body/string()");

         QStringList errormsgsResults;
         if (!query.evaluateTo(&errormsgsResults))
         {
             qDebug() << "parserMobileBahnDe::getJourneyDetailsData - Query 4 Failed";
         }

         QString info = results->info();

         if (info != "") {
             info += "<br>";
         }
         info += errormsgsResults.join("").trimmed();
         results->setInfo(info);
     }

     if (results->itemcount() > 0) {
         results->setDepartureStation(results->getItem(0)->departureStation());
         results->setArrivalStation(results->getItem(results->itemcount() - 1)->arrivalStation());

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

         results->setDuration(journeyDetailRequestData.duration);
         results->setId(journeyDetailRequestData.id);
     }

     emit journeyDetailsResult(results);
 }

