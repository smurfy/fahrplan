#include <QtGui/QApplication>
#include <QtDeclarative>
#include "Parser/parser_hafasxml.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<ParserHafasXml>("Fahrplan", 1, 0, "Parser");
    qmlRegisterType<StationsResultList>("Fahrplan", 1, 0, "StationsResultList");
    qmlRegisterType<StationsResultItem>("Fahrplan", 1, 0, "StationsResultItem");

    QDeclarativeView view;
    view.setSource(QUrl("qrc:/qml/main.qml"));
    view.showFullScreen();
    return app.exec();
}
