#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->findStationsByName, SIGNAL(clicked()), this, SLOT(findStationsByNameClicked()));
    connect(ui->findStationsByCoordinates, SIGNAL(clicked()), this, SLOT(findStationsByCoordinatesClicked()));

    backendParser = new ParserHafasXml();
    connect(backendParser, SIGNAL(stationsResult(StationsResultList*)), this, SLOT(stationsResult(StationsResultList*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::findStationsByNameClicked()
{
    backendParser->findStationsByName("München");
}

void MainWindow::findStationsByCoordinatesClicked()
{
    backendParser->findStationsByCoordinates(11.558338, 48.140228);
}

void MainWindow::stationsResult(StationsResultList *result)
{
    ui->findStationResults->clear();
    for (int i=0; i < result->itemcount(); i++) {
        StationsResultItem *item = result->getItem(i);
        ui->findStationResults->append(item->stationName());
    }
}
