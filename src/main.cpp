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

#include <QtCore/QTranslator>

#if defined(BUILD_FOR_QT5)
    #include <QtWidgets/QApplication>
#else
    #include <QtGui/QApplication>
#endif

#if defined(BUILD_FOR_HARMATTAN) || defined(BUILD_FOR_MAEMO_5) || defined(BUILD_FOR_SYMBIAN) || defined(BUILD_FOR_BLACKBERRY)
    #include <QtDeclarative>
#elif defined(BUILD_FOR_UBUNTU)
    #include <QtQuick>
    #include <QtQml>
#else
    #include "gui/desktop-test/mainwindow.h"
#endif

#if defined(HAVE_DECLARATIVE_CACHE)
    #include <MDeclarativeCache>
#endif

#if defined(BUILD_FOR_MAEMO_5)
    #include "gui/fremantle/hildon_helper.h"
#endif

#if defined(BUILD_FOR_BLACKBERRY)
    #include "blackberrypositionsource.h"
    #include <bps/geolocation.h>
    #include <QGLWidget>
#endif

#include "fahrplan.h"
#include "parser/parser_abstract.h"
#include "fahrplan_parser_thread.h"
#include "fahrplan_calendar_manager.h"
#include "models/stationsearchresults.h"
#include "models/favorites.h"
#include "models/timetable.h"
#include "models/trainrestrictions.h"

#if defined(BUILD_FOR_HARMATTAN) || defined(BUILD_FOR_MAEMO_5) || defined(BUILD_FOR_SYMBIAN)
Q_DECL_EXPORT
#endif
int main(int argc, char *argv[])
{
    #if defined(HAVE_DECLARATIVE_CACHE)
        QApplication* app = MDeclarativeCache::qApplication(argc, argv);
    #else
        QApplication* app = new QApplication(argc, argv);
    #endif

    // Install translations
    QTranslator translator;
    translator.load(QString("fahrplan_%1").arg(QLocale().name()), ":/translations");
    app->installTranslator(&translator);

    qDebug()<<"Startup";

    qRegisterMetaType<Station>();
    qRegisterMetaType<StationsList>();
    qRegisterMetaType<TimetableEntry>();
    qRegisterMetaType<TimetableEntriesList>();
    qRegisterMetaType<Fahrplan::StationType>();
    qRegisterMetaType<Fahrplan::Mode>();

    #if defined(BUILD_FOR_HARMATTAN) || defined(BUILD_FOR_MAEMO_5) || defined(BUILD_FOR_SYMBIAN) || defined(BUILD_FOR_BLACKBERRY) || defined(BUILD_FOR_UBUNTU)
        qDebug()<<"QML";
        qmlRegisterType<Fahrplan>("Fahrplan", 1, 0, "FahrplanBackend");
        qmlRegisterType<ParserAbstract>("Fahrplan", 1, 0, "ParserAbstract");
        qmlRegisterType<FahrplanParserThread>("Fahrplan", 1, 0, "FahrplanParserThread");
        qmlRegisterType<FahrplanCalendarManager>("Fahrplan", 1, 0, "CalendarManager");
        qmlRegisterUncreatableType<StationSearchResults>("Fahrplan", 1, 0, "StationSearchResults"
            , "StationSearchResults cannot be created from QML. "
              "Access it through FahrplanBackend.stationSearchResults.");
        qmlRegisterUncreatableType<Favorites>("Fahrplan", 1, 0, "Favorites"
            , "Favorites cannot be created from QML. "
              "Access it through FahrplanBackend.favorites.");
        qmlRegisterUncreatableType<Timetable>("Fahrplan", 1, 0, "Timetable"
            , "Timetable cannot be created from QML. "
              "Access it through FahrplanBackend.timetable.");
        qmlRegisterUncreatableType<Trainrestrictions>("Fahrplan", 1, 0, "Trainrestrictions"
            , "Trainrestrictions cannot be created from QML. "
              "Access it through FahrplanBackend.trainrestrictions.");
        qmlRegisterType<JourneyResultList>("Fahrplan", 1, 0, "JourneyResultList");
        qmlRegisterType<JourneyResultItem>("Fahrplan", 1, 0, "JourneyResultItem");
        qmlRegisterType<JourneyDetailResultList>("Fahrplan", 1, 0, "JourneyDetailResultList");
        qmlRegisterType<JourneyDetailResultItem>("Fahrplan", 1, 0, "JourneyDetailResultItem");

        #if defined(HAVE_DECLARATIVE_CACHE)
            QDeclarativeView* view = MDeclarativeCache::qDeclarativeView();
        #elif defined(BUILD_FOR_QT5)
            QQuickView *view = new QQuickView();
        #else
            QDeclarativeView* view = new QDeclarativeView();
        #endif

        #if defined(BUILD_FOR_HARMATTAN)
            qDebug()<<"Harmattan";
            view->setSource(QUrl("qrc:/src/gui/harmattan/main.qml"));
            view->showFullScreen();
        #elif defined(BUILD_FOR_MAEMO_5)
            qDebug()<<"Maemo5";
            qmlRegisterType<HildonHelper>("HildonHelper", 1, 0, "HildonHelper");
            view->setSource(QUrl("qrc:/src/gui/fremantle/main.qml"));
            view->show();
        #elif defined(BUILD_FOR_SYMBIAN)
            qDebug()<<"Symbian";
            view->setSource(QUrl("qrc:/src/gui/symbian/main.qml"));
            view->showFullScreen();
        #elif defined(BUILD_FOR_UBUNTU)
            qDebug()<<"Ubuntu";
            view->setSource(QUrl("qrc:/src/gui/ubuntu/main.qml"));
            view->setResizeMode(QQuickView::SizeRootObjectToView);
            if (QApplication::arguments().contains("--fullscreen")) {
                view->showFullScreen();
            } else {
                view->show();
            }
        #elif defined(BUILD_FOR_BLACKBERRY)
            qDebug() << "Blackberry";

            // QML wrapper around Qt Mobility Subset
            qmlRegisterType<QtMobilitySubset::BlackBerryPositionSource>("QtMobility.location", 1, 1, "PositionSource");
            qmlRegisterUncreatableType<QtMobilitySubset::BlackBerryPosition>("QtMobility.location", 1, 1, "Position", "Cant't create Position type");
            qmlRegisterUncreatableType<QtMobilitySubset::BlackBerryCoordinate>("QtMobility.location", 1, 1, "Coordinate", "Cant't create Coordinate type");

            QSettings *settings = new QSettings(FAHRPLAN_SETTINGS_NAMESPACE, "fahrplan2");

            // Check if GPS Location permission is set
            // and geolocation services are enabled.
            int res = geolocation_request_events(0);
            if (res == BPS_SUCCESS)
                geolocation_stop_events(0);
            settings->setValue("enableGps", res == BPS_SUCCESS);

            delete settings;

            // Improves touch handling
            QApplication::setStartDragDistance(42);

            QGLWidget *gl = new QGLWidget();
            view->setViewport(gl);
            view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
            view->setSource(QUrl("qrc:/src/gui/symbian/main.qml"));

            // Hide Symbian-style status bar on BlackBerry
            view->rootObject()->setProperty("showStatusBar", false);

            view->showFullScreen();
        #endif
    #else
        qDebug()<<"Desktop";
        MainWindow w;
        w.show();
    #endif

    qDebug()<<"Exec";

    return app->exec();
}
