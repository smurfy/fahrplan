/*******************************************************************************

    This file is a part of Fahrplan for maemo 2009-2011

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

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fahrplan = new Fahrplan();

    connect(ui->cancelRequest, SIGNAL(clicked()), this, SLOT(cancelRequestClicked()));
    connect(fahrplan, SIGNAL(parserErrorOccured(QString)), this, SLOT(errorOccured(QString)));

    connect(ui->findStationsByName, SIGNAL(clicked()), this, SLOT(findStationsByNameClicked()));
    connect(ui->findStationsByCoordinates, SIGNAL(clicked()), this, SLOT(findStationsByCoordinatesClicked()));
    connect(fahrplan, SIGNAL(parserStationsResult(StationsResultList*)), this, SLOT(stationsResult(StationsResultList*)));

    connect(ui->searchJourney, SIGNAL(clicked()), this, SLOT(searchJourneyClicked()));
    connect(ui->searchJourneyEarlier, SIGNAL(clicked()), this, SLOT(searchJourneyEarlierClicked()));
    connect(ui->searchJourneyLater, SIGNAL(clicked()), this, SLOT(searchJourneyLaterClicked()));
    connect(fahrplan, SIGNAL(parserJourneyResult(JourneyResultList*)), this, SLOT(journeyResult(JourneyResultList*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::errorOccured(QString msg)
{
    QMessageBox msgBox;
    msgBox.setText(msg);
    msgBox.exec();
}

void MainWindow::cancelRequestClicked()
{
    fahrplan->parser()->cancelRequest();
}

void MainWindow::findStationsByNameClicked()
{
    ui->findStationResults->clear();
    ui->findStationResults->append("Searching...");
    fahrplan->parser()->findStationsByName("München");
}

void MainWindow::findStationsByCoordinatesClicked()
{
    ui->findStationResults->clear();
    ui->findStationResults->append("Searching...");
    fahrplan->parser()->findStationsByCoordinates(11.558338, 48.140228);
}

void MainWindow::stationsResult(StationsResultList *result)
{
    ui->findStationResults->clear();
    for (int i=0; i < result->itemcount(); i++) {
        StationsResultItem *item = result->getItem(i);
        ui->findStationResults->append(item->stationName());
    }

    if (result->itemcount() == 0) {
        ui->findStationResults->append("No Results");
    }
}

void MainWindow::searchJourneyClicked()
{
    ui->searchJourneyResults->clear();
    ui->searchJourneyResults->append("Searching...");
    fahrplan->parser()->searchJourney(ui->departureStaion->text(), ui->arrivalStation->text(), "", QDate::currentDate(), QTime::currentTime(), 0, 0);
}

void MainWindow::searchJourneyEarlierClicked()
{
    ui->searchJourneyResults->clear();
    ui->searchJourneyResults->append("Searching...");
    fahrplan->parser()->searchJourneyEarlier();
}

void MainWindow::searchJourneyLaterClicked()
{
    ui->searchJourneyResults->clear();
    ui->searchJourneyResults->append("Searching...");
    fahrplan->parser()->searchJourneyLater();
}


void MainWindow::journeyResult(JourneyResultList *result)
{
    ui->searchJourneyResults->clear();

    ui->searchJourneyResults->append(result->departureStation());
    ui->searchJourneyResults->append(result->arrivalStation());
    ui->searchJourneyResults->append(result->timeInfo());

    for (int i=0; i < result->itemcount(); i++) {
        JourneyResultItem *item = result->getItem(i);
        ui->searchJourneyResults->append(">>-------------------------");
        ui->searchJourneyResults->append(item->id());
        ui->searchJourneyResults->append(item->date().toString());
        ui->searchJourneyResults->append(item->departureTime());
        ui->searchJourneyResults->append(item->arrivalTime());
        ui->searchJourneyResults->append(item->trainType());
        ui->searchJourneyResults->append(item->duration());
        ui->searchJourneyResults->append(item->transfers());
        ui->searchJourneyResults->append(item->miscInfo());
        ui->searchJourneyResults->append(item->internalData1());
        ui->searchJourneyResults->append(item->internalData2());
    }

    if (result->itemcount() == 0) {
        ui->searchJourneyResults->append("No Results");
    }
}
