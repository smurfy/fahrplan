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

#include <QtGui/QApplication>
#include <qplatformdefs.h>

#if defined(MEEGO_EDITION_HARMATTAN) || defined(Q_WS_MAEMO_5) || defined(Q_OS_SYMBIAN)
    #include <QtDeclarative>
#elif defined(Q_WS_WIN) || defined(Q_WS_X11)
    #include "gui/desktop-test/mainwindow.h"
#endif

#if defined(Q_WS_MAEMO_5)
    #include "gui/fremantle/hildon_helper.h"
#endif

#include "fahrplan.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Install translations
    QTranslator translator;
    translator.load(QString("fahrplan_%1").arg(QLocale::system().name()), ":/translations");
    app.installTranslator(&translator);

    #if defined(Q_WS_WIN)
        MainWindow w;
        w.show();
    #elif defined(Q_WS_X11) || defined(Q_OS_SYMBIAN)
        #if defined(MEEGO_EDITION_HARMATTAN) || defined(Q_WS_MAEMO_5) || defined(Q_OS_SYMBIAN)
            qmlRegisterType<Fahrplan>("Fahrplan", 1, 0, "FahrplanBackend");
            qmlRegisterType<ParserAbstract>("Fahrplan", 1, 0, "ParserAbstract");
            qmlRegisterType<FahrplanFavoritesManager>("Fahrplan", 1, 0, "FahrplanFavoritesManager");
            qmlRegisterType<StationsResultList>("Fahrplan", 1, 0, "StationsResultList");
            qmlRegisterType<StationsResultItem>("Fahrplan", 1, 0, "StationsResultItem");
            qmlRegisterType<JourneyResultList>("Fahrplan", 1, 0, "JourneyResultList");
            qmlRegisterType<JourneyResultItem>("Fahrplan", 1, 0, "JourneyResultItem");
            qmlRegisterType<JourneyDetailResultList>("Fahrplan", 1, 0, "JourneyDetailResultList");
            qmlRegisterType<JourneyDetailResultItem>("Fahrplan", 1, 0, "JourneyDetailResultItem");
            qmlRegisterType<TimeTableResultList>("Fahrplan", 1, 0, "TimeTableResultList");
            qmlRegisterType<TimeTableResultItem>("Fahrplan", 1, 0, "TimeTableResultItem");
            QDeclarativeView view;
        #endif

        #if defined(MEEGO_EDITION_HARMATTAN)
            view.setSource(QUrl("qrc:/src/gui/harmattan/main.qml"));
            view.showFullScreen();
        #elif defined(Q_WS_MAEMO_5)
            qmlRegisterType<HildonHelper>("HildonHelper", 1, 0, "HildonHelper");
            view.setSource(QUrl("qrc:/src/gui/fremantle/main.qml"));
            view.show();
        #elif defined(Q_OS_SYMBIAN)
            view.setSource(QUrl("qrc:/src/gui/symbian/main.qml"));
            view.showFullScreen();
        #else
            MainWindow w;
            w.show();
        #endif
    #else
        //Unknown
    #endif

    return app.exec();
}
