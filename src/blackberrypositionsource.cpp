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

#include "blackberrypositionsource.h"
#include <QtLocationSubset/QGeoPositionInfoSource>
#include <QtLocationSubset/QGeoPositionInfo>

#include <QDebug>

namespace QtMobilitySubset {

BlackBerryCoordinate::BlackBerryCoordinate(const QGeoCoordinate &coordinate, QObject *parent)
    : QObject(parent), m_coordinate(coordinate) {}

double BlackBerryCoordinate::latitude() const
{
    return m_coordinate.latitude();
}

double BlackBerryCoordinate::longitude() const
{
    return m_coordinate.longitude();
}

double BlackBerryCoordinate::altitude() const
{
    return m_coordinate.altitude();
}

BlackBerryPosition::BlackBerryPosition(const QGeoPositionInfo &position, QObject *parent)
    : QObject(parent), m_coordinate(new BlackBerryCoordinate(position.coordinate(), this))
    , m_position(position) {}

bool BlackBerryPosition::latitudeValid() const
{
    return m_position.isValid();
}

bool BlackBerryPosition::longitudeValid() const
{
    return m_position.isValid();
}

bool BlackBerryPosition::altitudeValid() const
{
    return m_position.coordinate().type() == QGeoCoordinate::Coordinate3D;
}

QDateTime BlackBerryPosition::timestamp() const
{
    return m_position.timestamp();
}

BlackBerryCoordinate *BlackBerryPosition::coordinate() const
{
    return m_coordinate;
}

BlackBerryPositionSource::BlackBerryPositionSource(QObject *parent)
    : QObject(parent), m_active(false), m_position(NULL)
{
    positionSource = QGeoPositionInfoSource::createDefaultSource(this);
    connect(positionSource, SIGNAL(positionUpdated(const QGeoPositionInfo &)), SLOT(positionUpdated(const QGeoPositionInfo &)));
    connect(positionSource, SIGNAL(updateTimeout()), SLOT(updateTimeout()));
}

bool BlackBerryPositionSource::active() const
{
    return m_active;
}

void BlackBerryPositionSource::setActive(bool active)
{
    if (active == m_active)
        return;
    m_active = active;
    if (m_active)
        positionSource->startUpdates();
    else
        positionSource->stopUpdates();
    emit activeChanged();
}

void BlackBerryPositionSource::start()
{
    setActive(true);
}

void BlackBerryPositionSource::stop()
{
    setActive(false);
}

void BlackBerryPositionSource::update()
{
    positionSource->requestUpdate(30000);
}

BlackBerryPosition *BlackBerryPositionSource::position() const
{
    return m_position;
}

void BlackBerryPositionSource::positionUpdated(const QGeoPositionInfo &update)
{
    qDebug() << "Position updated:" << update.coordinate().toString();
    if (m_position)
        m_position->deleteLater();
    m_position = new BlackBerryPosition(update, this);
    emit positionChanged();
}

void BlackBerryPositionSource::updateTimeout()
{
    qDebug() << "Geo position update Timeout!";
}

}
