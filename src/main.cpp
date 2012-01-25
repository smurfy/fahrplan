#include <QtGui/QApplication>
#include <qplatformdefs.h>

#if defined(MEEGO_EDITION_HARMATTAN) || defined(Q_WS_MAEMO_5)
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
    #elif defined(Q_WS_X11)
        #if defined(MEEGO_EDITION_HARMATTAN) || defined(Q_WS_MAEMO_5)
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
        #else
            MainWindow w;
            w.show();
        #endif
    #else
        //Unknown
    #endif

    return app.exec();
}
