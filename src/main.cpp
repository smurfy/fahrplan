#include <QtGui/QApplication>
#include <qplatformdefs.h> // MEEGO_EDITION_HARMATTAN

#if defined(Q_WS_MAEMO_5)
    #include <QtDBus>
#endif

#if defined(MEEGO_EDITION_HARMATTAN) || defined(Q_WS_MAEMO_5)
    #include <QtDeclarative>
#elif defined(Q_WS_WIN) || defined(Q_WS_X11)
    #include "gui/desktop-test/mainwindow.h"
#endif

#include "fahrplan.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    #if defined(MEEGO_EDITION_HARMATTAN) || defined(Q_WS_MAEMO_5)
        qmlRegisterType<Fahrplan>("Fahrplan", 1, 0, "Backend");
        qmlRegisterType<ParserAbstract>("Fahrplan", 1, 0, "ParserAbstract");
        qmlRegisterType<FahrplanFavoritesManager>("Fahrplan", 1, 0, "FahrplanFavoritesManager");
        qmlRegisterType<StationsResultList>("Fahrplan", 1, 0, "StationsResultList");
        qmlRegisterType<StationsResultItem>("Fahrplan", 1, 0, "StationsResultItem");
        qmlRegisterType<JourneyResultList>("Fahrplan", 1, 0, "JourneyResultList");
        qmlRegisterType<JourneyResultItem>("Fahrplan", 1, 0, "JourneyResultItem");
        qmlRegisterType<JourneyDetailResultList>("Fahrplan", 1, 0, "JourneyDetailResultList");
        qmlRegisterType<JourneyDetailResultItem>("Fahrplan", 1, 0, "JourneyDetailResultItem");

        QDeclarativeView view;
        #if defined(MEEGO_EDITION_HARMATTAN)
            view.setSource(QUrl("qrc:/src/gui/harmattan/main.qml"));
            view.showFullScreen();
        #endif
        #if defined(Q_WS_MAEMO_5)
            view.setSource(QUrl("qrc:/src/gui/fremantle/main.qml"));
            view.show();
        #endif

    #elif defined(Q_WS_WIN) || defined(Q_WS_X11)
        MainWindow w;
        w.show();
    #endif

    return app.exec();
}
