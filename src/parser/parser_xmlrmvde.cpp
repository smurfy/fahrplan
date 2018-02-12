#include "parser_xmlrmvde.h"

#include <QNetworkReply>

ParserXmlRMVde::ParserXmlRMVde(QObject *parent) :
    ParserHafasXml(parent)
{
    baseXmlUrl = QLatin1String("http://www.rmv.de/auskunft/bin/jp/query.exe");
    baseUrl = QLatin1String("http://www.rmv.de/auskunft/bin/jp/query.exe");
}

void ParserXmlRMVde::parseStationsByName(QNetworkReply *networkReply)
{
    const QString data = QString::fromUtf8(networkReply->readAll());
    const StationsList result = internalParseStationsByName(data);
    emit stationsResult(result);
}

QString ParserXmlRMVde::getTrainRestrictionsCodes(int trainrestrictions)
{
    Q_UNUSED(trainrestrictions)
    // TODO should something else get returned?
    return "1111111111111111";
}

QStringList ParserXmlRMVde::getTrainRestrictions()
{
    QStringList result;
    result.append(tr("All"));
    return result;
}
