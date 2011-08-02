#include "parser_definitions.h"

QString StationsResultItem::stationName() const
{
    return m_stationName;
}

void StationsResultItem::setStationName(const QString &stationName)
{
    m_stationName = stationName;
}

QString StationsResultItem::stationType() const
{
    return m_stationName;
}

void StationsResultItem::setStationType(const QString &stationType)
{
    m_stationType = stationType;
}

QString StationsResultItem::miscInfo() const
{
    return m_miscInfo;
}

void StationsResultItem::setMiscInfo(const QString &miscInfo)
{
    m_miscInfo = miscInfo;
}

qreal StationsResultItem::longitude()
{
    return m_longitude;
}

void StationsResultItem::setLongitude(qreal longitude)
{
    m_longitude = longitude;
}

qreal StationsResultItem::latitude()
{
    return m_latitude;
}

void StationsResultItem::setLatitude(qreal latitude)
{
    m_latitude = latitude;
}

qreal StationsResultList::itemcount()
{
    return m_items.count();
}

StationsResultItem *StationsResultList::getItem(int index)
{
    return  m_items.at(index);
}

void StationsResultList::appendItem(StationsResultItem *item)
{
    m_items.append(item);
}
