#include <QtGui/QApplication>

#ifdef Q_WS_X11
    #include <QtDeclarative>
#endif

#ifdef Q_WS_WIN
    #include "gui/desktop-test/mainwindow.h"
#endif

#include "fahrplan.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    #ifdef Q_WS_MAEMO_5
        //Not Yet!
    #endif

    #ifdef Q_WS_X11
        //It seems, that there is currently no way of telling if its
        //On harmattan or linux itself so i use the x11 switch here, because i use win for my development

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
        view.setSource(QUrl("qrc:/src/gui/harmattan/main.qml"));
        view.showFullScreen();
    #endif

    #ifdef Q_WS_WIN
        MainWindow w;
        w.show();
    #endif

    return app.exec();
}
