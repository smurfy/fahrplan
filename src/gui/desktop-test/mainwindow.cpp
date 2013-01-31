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

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fahrplan = new Fahrplan();

    connect(ui->cancelRequest, SIGNAL(clicked()), this, SLOT(cancelRequestClicked()));
    connect(fahrplan, SIGNAL(parserErrorOccured(const QString &)), this, SLOT(errorOccured(const QString &)));

    connect(ui->getTimeTableForStation, SIGNAL(clicked()), this, SLOT(getTimeTableForStationClicked()));
    connect(ui->findStationsByName, SIGNAL(clicked()), this, SLOT(findStationsByNameClicked()));
    connect(ui->findStationsByCoordinates, SIGNAL(clicked()), this, SLOT(findStationsByCoordinatesClicked()));
    connect(fahrplan, SIGNAL(parserStationsResult(StationsResultList*)), this, SLOT(stationsResult(StationsResultList*)));
    connect(fahrplan, SIGNAL(parserTimeTableResult(TimeTableResultList*)), this, SLOT(timeTableResult(TimeTableResultList*)));

    connect(ui->searchJourney, SIGNAL(clicked()), this, SLOT(searchJourneyClicked()));
    connect(ui->searchJourneyEarlier, SIGNAL(clicked()), this, SLOT(searchJourneyEarlierClicked()));
    connect(ui->searchJourneyLater, SIGNAL(clicked()), this, SLOT(searchJourneyLaterClicked()));
    connect(fahrplan, SIGNAL(parserJourneyResult(JourneyResultList*)), this, SLOT(journeyResult(JourneyResultList*)));

    connect(ui->getJourneyDetails, SIGNAL(clicked()), this, SLOT(getJourneyDetailsClicked()));
    connect(fahrplan, SIGNAL(parserJourneyDetailsResult(JourneyDetailResultList*)), this, SLOT(journeyDetailResult(JourneyDetailResultList*)));

    connect(fahrplan, SIGNAL(parserChanged(const QString &, int)), this, SLOT(parserChanged(const QString &, int)));

    QStringListModel *parserModel = new QStringListModel();
    parserModel->setStringList(fahrplan->getParserList());
    ui->parser->setModel(parserModel);

    connect(ui->parser, SIGNAL( currentIndexChanged(int)), this, SLOT( parserCurrentIndexChanged(int)));

    fahrplan->parser();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::parserCurrentIndexChanged(int index)
{
    fahrplan->setParser(index);
}

void MainWindow::parserChanged(const QString &name, int index)
{
    setWindowTitle("Fahrplan - TestApp - " + name + " - " + QString::number(index));

    ui->parser->setCurrentIndex(index);

    QStringList trainRestrictions = fahrplan->parser()->getTrainRestrictions();
    ui->trainRestrictions->setEnabled(false);
    if (trainRestrictions.count() > 0)
    {
        QStringListModel *trainrestrModel = new QStringListModel();
        trainrestrModel->setStringList(trainRestrictions);
        ui->trainRestrictions->setModel(trainrestrModel);
        ui->trainRestrictions->setEnabled(true);
        ui->trainRestrictions->setCurrentIndex(0);
    }

    ui->viaStation->setEnabled(fahrplan->parser()->supportsVia());
    ui->getTimeTableForStation->setEnabled(fahrplan->parser()->supportsTimeTable());
}

void MainWindow::errorOccured(const QString &msg)
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
    fahrplan->parser()->findStationsByName(ui->stationName->text());
}

void MainWindow::getTimeTableForStationClicked()
{
    ui->findStationResults->clear();
    ui->findStationResults->append("Searching...");
    fahrplan->parser()->getTimeTableForStation(ui->stationName->text(), "", QDate::currentDate(), QTime::currentTime(), ParserAbstract::Departure, 0);
}

void MainWindow::findStationsByCoordinatesClicked()
{
    ui->findStationResults->clear();
    ui->findStationResults->append("Searching...");
    fahrplan->parser()->findStationsByCoordinates(11.558338, 48.140228);
}

void MainWindow::timeTableResult(TimeTableResultList *result)
{
    ui->findStationResults->clear();
    for (int i=0; i < result->itemcount(); i++) {
        TimeTableResultItem *item = result->getItem(i);
        ui->findStationResults->append(item->stationName());
        ui->findStationResults->append(item->time().toString(tr("hh:mm")));
        ui->findStationResults->append(item->destinationName());
        ui->findStationResults->append(item->trainType());
        ui->findStationResults->append(item->platform());
        ui->findStationResults->append("------------------------------");
    }

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
    fahrplan->parser()->searchJourney(ui->departureStaion->text(), ui->arrivalStation->text(), ui->viaStation->text(), QDate::currentDate(), QTime::currentTime(), ParserAbstract::Departure, ui->trainRestrictions->currentIndex());
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
    ui->journeyResultItemIds->clear();

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

        ui->journeyResultItemIds->addItem(item->id());
    }

    if (result->itemcount() == 0) {
        ui->searchJourneyResults->append("No Results");
    }
}

void MainWindow::getJourneyDetailsClicked()
{
    ui->getJourneyDetailsResults->clear();
    ui->getJourneyDetailsResults->append("Loading...");
    fahrplan->parser()->getJourneyDetails(ui->journeyResultItemIds->currentText());
}

void MainWindow::journeyDetailResult(JourneyDetailResultList *result)
{
    ui->getJourneyDetailsResults->clear();
    ui->getJourneyDetailsResults->append(result->departureStation());
    ui->getJourneyDetailsResults->append(result->departureDateTime().toString());
    ui->getJourneyDetailsResults->append(result->arrivalStation());
    ui->getJourneyDetailsResults->append(result->arrivalDateTime().toString());
    ui->getJourneyDetailsResults->append(result->duration());
    ui->getJourneyDetailsResults->append(result->info());

    for (int i=0; i < result->itemcount(); i++) {
        JourneyDetailResultItem *item = result->getItem(i);
        ui->getJourneyDetailsResults->append(">>-------------------------");
        ui->getJourneyDetailsResults->append(item->departureStation());
        ui->getJourneyDetailsResults->append(item->departureDateTime().toString());
        ui->getJourneyDetailsResults->append(item->departureInfo());
        ui->getJourneyDetailsResults->append(item->arrivalStation());
        ui->getJourneyDetailsResults->append(item->arrivalDateTime().toString());
        ui->getJourneyDetailsResults->append(item->arrivalInfo());
        ui->getJourneyDetailsResults->append(item->train());
        ui->getJourneyDetailsResults->append(item->info());
        ui->getJourneyDetailsResults->append(item->internalData1());
        ui->getJourneyDetailsResults->append(item->internalData2());
    }

    if (result->itemcount() == 0) {
        ui->getJourneyDetailsResults->append("No Results");
    }
}
