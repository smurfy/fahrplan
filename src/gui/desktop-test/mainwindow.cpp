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
    connect(fahrplan, SIGNAL(parserStationsResult()), this, SLOT(stationsResult()));
    connect(fahrplan, SIGNAL(parserTimeTableResult()), this, SLOT(timeTableResult()));

    connect(ui->searchJourney, SIGNAL(clicked()), this, SLOT(searchJourneyClicked()));
    connect(ui->searchJourneyEarlier, SIGNAL(clicked()), this, SLOT(searchJourneyEarlierClicked()));
    connect(ui->searchJourneyLater, SIGNAL(clicked()), this, SLOT(searchJourneyLaterClicked()));
    connect(fahrplan, SIGNAL(parserJourneyResult()), this, SLOT(journeyResult()));

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

    ui->trainRestrictions->setEnabled(false);
    if (fahrplan->trainrestrictions()->count() > 0)
    {
        ui->trainRestrictions->setModel(fahrplan->trainrestrictions());
        ui->trainRestrictions->setEnabled(true);
        ui->trainRestrictions->setCurrentIndex(0);
    }

    //ui->viaStation->setEnabled(fahrplan->parser()->supportsVia());
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
    fahrplan->findStationsByName(ui->stationName->text());
}

void MainWindow::getTimeTableForStationClicked()
{
    ui->findStationResults->clear();
    ui->findStationResults->append("Searching...");
    fahrplan->stationSearchResults()->selectStation(Fahrplan::CurrentStation, ui->stationIndex->value());
    fahrplan->getTimeTable();
}

void MainWindow::findStationsByCoordinatesClicked()
{
    ui->findStationResults->clear();
    ui->findStationResults->append("Searching...");
    fahrplan->findStationsByCoordinates(11.558338, 48.140228);
}

void MainWindow::timeTableResult()
{
    ui->findStationResults->clear();
    for (int i=0; i < fahrplan->timetable()->count(); i++) {
        QModelIndex index = fahrplan->timetable()->index(i, 0, QModelIndex());
        ui->findStationResults->append(fahrplan->timetable()->data(index, Timetable::CurrentStation).toString());
        ui->findStationResults->append(fahrplan->timetable()->data(index, Timetable::Time).toDateTime().toString(tr("hh:mm")));
        ui->findStationResults->append(fahrplan->timetable()->data(index, Timetable::DestinationStation).toString());
        ui->findStationResults->append(fahrplan->timetable()->data(index, Timetable::TrainType).toString());
        ui->findStationResults->append(fahrplan->timetable()->data(index, Timetable::Platform).toString());
        ui->findStationResults->append("------------------------------");
    }

}

void MainWindow::stationsResult()
{
    ui->findStationResults->clear();
    for (int i=0; i < fahrplan->stationSearchResults()->count(); i++) {
        QModelIndex index = fahrplan->stationSearchResults()->index(i, 0, QModelIndex());
        ui->findStationResults->append(QString::number(i) + " -> " + fahrplan->stationSearchResults()->data(index, StationSearchResults::Name).toString());
    }

    if (fahrplan->stationSearchResults()->count() == 0) {
        ui->findStationResults->append("No Results");
    }
}

void MainWindow::searchJourneyClicked()
{
    ui->searchJourneyResults->clear();
    ui->searchJourneyResults->append("Searching...");
    fahrplan->setDateTime(QDateTime::currentDateTime());
    fahrplan->setMode(Fahrplan::DepartureMode);
    fahrplan->stationSearchResults()->selectStation(Fahrplan::DepartureStation, 0);
    fahrplan->stationSearchResults()->selectStation(Fahrplan::ArrivalStation, 10);
    fahrplan->searchJourney();
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


void MainWindow::journeyResult()
{
    ui->searchJourneyResults->clear();
    ui->journeyResultItemIds->clear();

    ui->searchJourneyResults->append(fahrplan->journeyresults()->departureStation().name);
    ui->searchJourneyResults->append(fahrplan->journeyresults()->arrivalStation().name);
    ui->searchJourneyResults->append(fahrplan->journeyresults()->timeInfo());


    for (int i=0; i < fahrplan->journeyresults()->count(); i++) {
        QModelIndex index = fahrplan->journeyresults()->index(i, 0, QModelIndex());
        ui->searchJourneyResults->append(">>-------------------------");
        ui->searchJourneyResults->append(fahrplan->journeyresults()->data(index, JourneyResults::Id).toString());
        ui->searchJourneyResults->append(fahrplan->journeyresults()->data(index, JourneyResults::Date).toDate().toString());
        ui->searchJourneyResults->append(fahrplan->journeyresults()->data(index, JourneyResults::DepartureTime).toString());
        ui->searchJourneyResults->append(fahrplan->journeyresults()->data(index, JourneyResults::ArrivalTime).toString());
        ui->searchJourneyResults->append(fahrplan->journeyresults()->data(index, JourneyResults::TrainType).toString());
        ui->searchJourneyResults->append(fahrplan->journeyresults()->data(index, JourneyResults::Duration).toString());
        ui->searchJourneyResults->append(fahrplan->journeyresults()->data(index, JourneyResults::Transfers).toString());
        ui->searchJourneyResults->append(fahrplan->journeyresults()->data(index, JourneyResults::MiscInfo).toString());
        ui->journeyResultItemIds->addItem(fahrplan->journeyresults()->data(index, JourneyResults::Id).toString());
    }

    if (fahrplan->journeyresults()->count() == 0) {
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
    ui->getJourneyDetailsResults->append(result->departureStation().name);
    ui->getJourneyDetailsResults->append(result->departureStation().departureDateTime.toString());
    ui->getJourneyDetailsResults->append(result->arrivalStation().name);
    ui->getJourneyDetailsResults->append(result->arrivalStation().arrivalDateTime.toString());
    ui->getJourneyDetailsResults->append(result->duration());
    ui->getJourneyDetailsResults->append(result->info());

    for (int i=0; i < result->itemcount(); i++) {
        JourneyDetailResultItem *item = result->getItem(i);
        ui->getJourneyDetailsResults->append(">>-------------------------");
        ui->getJourneyDetailsResults->append(item->departureStation().name);
        ui->getJourneyDetailsResults->append(item->departureStation().departureDateTime.toString());
        ui->getJourneyDetailsResults->append(item->departureInfo());
        ui->getJourneyDetailsResults->append(item->arrivalStation().name);
        ui->getJourneyDetailsResults->append(item->arrivalStation().arrivalDateTime.toString());
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
