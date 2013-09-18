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

#include "parser_definitions.h"


//-------------- Station

Station::Station()
    : valid(true)
    , latitude(0)
    , longitude(0)
{}

Station::Station(bool isValid)
    : valid(isValid)
    , latitude(0)
    , longitude(0)
{}

bool Station::operator ==(const Station &other) const
{
    if (valid != other.valid)
        return false;
    if (id != other.id)
        return false;
    // Comparing ID should be enough, cause it's uniquie identifier
    // of the station. Everything else is details.
//    if (name != other.name)
//        return false;
//    if (type != other.type)
//        return false;
//    if (miscInfo != other.miscInfo)
//        return false;
//    if (qFuzzyCompare(latitude, other.latitude))
//        return false;
//    if (qFuzzyCompare(longitude, other.longitude))
//        return false;
    return true;
}

bool Station::operator <(const Station &other) const
{
    return name < other.name;
}

StopStation &StopStation::operator=(const Station& station)
{
    return (StopStation&)station;
}


//-------------- TimetableEntry

TimetableEntry::TimetableEntry()
{}


//------------- JourneyResultList

qreal JourneyResultList::itemcount()
{
    return m_items.count();
}

JourneyResultItem *JourneyResultList::getItem(int index)
{
    return  m_items.at(index);
}

void JourneyResultList::appendItem(JourneyResultItem *item)
{
    m_items.append(item);
}

Station &JourneyResultList::departureStation()
{
    return m_departureStation;
}

void JourneyResultList::setDepartureStation(const Station &departureStation)
{
    m_departureStation = departureStation;
}

Station &JourneyResultList::viaStation()
{
    return m_viaStation;
}

void JourneyResultList::setViaStation(const Station &viaStation)
{
    m_viaStation = viaStation;
}

Station &JourneyResultList::arrivalStation()
{
    return m_arrivalStation;
}

void JourneyResultList::setArrivalStation(const Station &arrivalStation)
{
    m_arrivalStation = arrivalStation;
}

QString JourneyResultList::timeInfo() const
{
    return m_timeInfo;
}

void JourneyResultList::setTimeInfo(const QString &timeInfo)
{
    m_timeInfo = timeInfo;
}

//------------- JourneyResultItem

QString JourneyResultItem::id() const
{
    return m_id;
}

void JourneyResultItem::setId(const QString &id)
{
    m_id = id;
}

QDate JourneyResultItem::date() const
{
    return m_date;
}

void JourneyResultItem::setDate(const QDate &date)
{
    m_date = date;
}

QString JourneyResultItem::departureTime() const
{
    return m_departureTime;
}

void JourneyResultItem::setDepartureTime(const QString &departureTime)
{
    m_departureTime = departureTime;
}

QString JourneyResultItem::arrivalTime() const
{
    return m_arrivalTime;
}

void JourneyResultItem::setArrivalTime(const QString &arrivalTime)
{
    m_arrivalTime = arrivalTime;
}

QString JourneyResultItem::trainType() const
{
    return m_trainType;
}

void JourneyResultItem::setTrainType(const QString &trainType)
{
    m_trainType = trainType;
}

QString JourneyResultItem::duration() const
{
    return m_duration;
}

void JourneyResultItem::setDuration(const QString &duration)
{
    m_duration = duration;
}

QString JourneyResultItem::transfers() const
{
    return m_transfers;
}

void JourneyResultItem::setTransfers(const QString &transfers)
{
    m_transfers = transfers;
}

QString JourneyResultItem::miscInfo() const
{
    return m_miscInfo;
}

void JourneyResultItem::setMiscInfo(const QString &miscInfo)
{
    m_miscInfo = miscInfo;
}

QString JourneyResultItem::internalData1() const
{
    return m_internalData1;
}

void JourneyResultItem::setInternalData1(const QString &internalData1)
{
    m_internalData1 = internalData1;
}

QString JourneyResultItem::internalData2() const
{
    return m_internalData2;
}

void JourneyResultItem::setInternalData2(const QString &internalData2)
{
    m_internalData2 = internalData2;
}

//------------- JourneyDetailResultList

QString JourneyDetailResultList::id() const
{
    return m_id;
}

void JourneyDetailResultList::setId(const QString &id)
{
    m_id = id;
}

qreal JourneyDetailResultList::itemcount()
{
    return m_items.count();
}

JourneyDetailResultItem *JourneyDetailResultList::getItem(int index)
{
    return  m_items.at(index);
}

void JourneyDetailResultList::appendItem(JourneyDetailResultItem *item)
{
    m_items.append(item);
}

StopStation &JourneyDetailResultList::departureStation()
{
    return m_departureStation;
}

void JourneyDetailResultList::setDepartureStation(const StopStation &departureStation)
{
    m_departureStation = departureStation;
}

StopStation &JourneyDetailResultList::viaStation()
{
    return m_viaStation;
}

void JourneyDetailResultList::setViaStation(const StopStation &viaStation)
{
    m_viaStation = viaStation;
}

StopStation &JourneyDetailResultList::arrivalStation()
{
    return m_arrivalStation;
}

void JourneyDetailResultList::setArrivalStation(const StopStation &arrivalStation)
{
    m_arrivalStation = arrivalStation;
}

QString JourneyDetailResultList::info() const
{
    return m_info;
}

void JourneyDetailResultList::setInfo(const QString &info)
{
    m_info = info;
}

QString JourneyDetailResultList::duration() const
{
    return m_duration;
}

void JourneyDetailResultList::setDuration(const QString &duration)
{
    m_duration = duration;
}

//------------- JourneyDetailResultItem

StopStation &JourneyDetailResultItem::departureStation()
{
    return m_departureStation;
}

void JourneyDetailResultItem::setDepartureStation(const StopStation &departureStation)
{
    m_departureStation = departureStation;
}

QString JourneyDetailResultItem::departureInfo() const
{
    return m_departureInfo;
}

void JourneyDetailResultItem::setDepartureInfo(const QString &departureInfo)
{
    m_departureInfo = departureInfo;
}

StopStation &JourneyDetailResultItem::arrivalStation()
{
    return m_arrivalStation;
}

void JourneyDetailResultItem::setArrivalStation(const StopStation &arrivalStation)
{
    m_arrivalStation = arrivalStation;
}

QString JourneyDetailResultItem::arrivalInfo() const
{
    return m_arrivalInfo;
}

void JourneyDetailResultItem::setArrivalInfo(const QString &arrivalInfo)
{
    m_arrivalInfo = arrivalInfo;
}

QString JourneyDetailResultItem::info() const
{
    return m_info;
}

void JourneyDetailResultItem::setInfo(const QString &info)
{
    m_info = info;
}

QString JourneyDetailResultItem::train() const
{
    return m_train;
}

void JourneyDetailResultItem::setTrain(const QString &train)
{
    m_train = train;
}

QString JourneyDetailResultItem::internalData1() const
{
    return m_internalData1;
}

void JourneyDetailResultItem::setInternalData1(const QString &internalData1)
{
    m_internalData1 = internalData1;
}

QString JourneyDetailResultItem::internalData2() const
{
    return m_internalData2;
}

void JourneyDetailResultItem::setInternalData2(const QString &internalData2)
{
    m_internalData2 = internalData2;
}
