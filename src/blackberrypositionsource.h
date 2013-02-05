#ifndef BLACKBERRYPOSITIONSOURCE_H
#define BLACKBERRYPOSITIONSOURCE_H

#include <QObject>
#include <QDateTime>
#include <QtLocationSubset/QGeoPositionInfo>
#include <QtLocationSubset/QGeoCoordinate>

namespace QtMobilitySubset {

class QGeoPositionInfoSource;

class BlackBerryCoordinate: public QObject
{
    Q_OBJECT

    Q_PROPERTY(double latitude READ latitude)
    Q_PROPERTY(double longitude READ longitude)
    Q_PROPERTY(double altitude READ altitude)

public:
    BlackBerryCoordinate(const QGeoCoordinate &coordinate, QObject *parent = 0);

    double latitude() const;
    double longitude() const;
    double altitude() const;

private:
    QGeoCoordinate m_coordinate;
};

class BlackBerryPosition: public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool latitudeValid READ latitudeValid)
    Q_PROPERTY(bool longitudeValid READ longitudeValid)
    Q_PROPERTY(bool altitudeValid READ altitudeValid)
    Q_PROPERTY(QDateTime timestamp READ timestamp)
    Q_PROPERTY(QtMobilitySubset::BlackBerryCoordinate *coordinate READ coordinate)

public:
    BlackBerryPosition(const QGeoPositionInfo &position, QObject *parent = 0);
    bool latitudeValid() const;
    bool longitudeValid() const;
    bool altitudeValid() const;
    QDateTime timestamp() const;
    BlackBerryCoordinate* coordinate() const;

    void setCoordinate(QGeoCoordinate coordinate);
    void setTimestamp(const QDateTime& timestamp);

private:
    BlackBerryCoordinate *m_coordinate;
    QGeoPositionInfo m_position;
};

class BlackBerryPositionSource: public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(QtMobilitySubset::BlackBerryPosition *position READ position NOTIFY positionChanged)

public:
    BlackBerryPositionSource(QObject *parent = 0);

    bool active() const;
    void setActive(bool active);
    BlackBerryPosition *position() const;

public slots:
    void start();
    void stop();
    void update();

signals:
    void activeChanged();
    void positionChanged();

private:
    bool m_active;
    BlackBerryPosition *m_position;

    QGeoPositionInfoSource *positionSource;

private slots:
    void positionUpdated(const QGeoPositionInfo &update);
    void updateTimeout();
};

}

#endif // BLACKBERRYPOSITIONSOURCE_H
