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

#include <QScrollBar>

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
    connect(fahrplan, SIGNAL(parserJourneyResult(JourneyResultList*)), this, SLOT(journeyResult(JourneyResultList*)));

    connect(ui->getJourneyDetails, SIGNAL(clicked()), this, SLOT(getJourneyDetailsClicked()));
    connect(fahrplan, SIGNAL(parserJourneyDetailsResult(JourneyDetailResultList*)), this, SLOT(journeyDetailResult(JourneyDetailResultList*)));

    connect(ui->selectStation, SIGNAL(clicked()), this, SLOT(selectStationClicked()));

    connect(fahrplan, SIGNAL(parserChanged(const QString &, int)), this, SLOT(parserChanged(const QString &, int)));

    if (fahrplan->backends()->count() > 0)
    {
        ui->parser->clear();
        for (int i=0; i < fahrplan->backends()->count(); i++) {
            QModelIndex index = fahrplan->backends()->index(i, 0, QModelIndex());
            ui->parser->addItem(fahrplan->backends()->data(index, Backends::Name).toString());
        }
        ui->parser->setCurrentIndex(0);
    }

    ui->stationType->addItem("Departure", Fahrplan::DepartureStation);
    ui->stationType->addItem("Via", Fahrplan::ViaStation);
    ui->stationType->addItem("Arrival", Fahrplan::ArrivalStation);
    ui->stationType->addItem("Current", Fahrplan::CurrentStation);
    ui->stationType->addItem("Direction", Fahrplan::DirectionStation);
    ui->stationsList->setModel(fahrplan->stationSearchResults());

    connect(ui->parser, SIGNAL( currentIndexChanged(int)), this, SLOT( parserCurrentIndexChanged(int)));

    fahrplan->parser();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::parserCurrentIndexChanged(int index)
{
    fahrplan->setParser(fahrplan->backends()->getParserIdForItemIndex(index));
}

void MainWindow::parserChanged(const QString &name, int index)
{
    setWindowTitle("Fahrplan - TestApp - " + name + " - " + QString::number(index));

    ui->parser->setCurrentIndex(fahrplan->backends()->getItemIndexForParserId(index));

    ui->trainRestrictions->setEnabled(false);
    qDebug()<<fahrplan->trainrestrictions()->count();
    if (fahrplan->trainrestrictions()->count() > 0)
    {
        ui->trainRestrictions->clear();
        for (int i=0; i < fahrplan->trainrestrictions()->count(); i++) {
            QModelIndex index = fahrplan->trainrestrictions()->index(i, 0, QModelIndex());
            ui->trainRestrictions->addItem(fahrplan->trainrestrictions()->data(index,Qt::DisplayRole).toString());
        }

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
    ui->searchJourneyResults->clear();
    ui->searchJourneyResults->append("Searching...");
    fahrplan->findStationsByName(ui->stationName->text());
}

void MainWindow::getTimeTableForStationClicked()
{
    ui->searchJourneyResults->clear();
    ui->searchJourneyResults->append("Searching...");
    fahrplan->setMode(Fahrplan::NowMode);
    fahrplan->getTimeTable();
}

void MainWindow::findStationsByCoordinatesClicked()
{
    ui->searchJourneyResults->clear();
    ui->searchJourneyResults->append("Searching...");
    fahrplan->findStationsByCoordinates(11.558338, 48.140228);
}

void MainWindow::timeTableResult()
{
    ui->searchJourneyResults->clear();
    for (int i=0; i < fahrplan->timetable()->count(); i++) {
        QModelIndex index = fahrplan->timetable()->index(i, 0, QModelIndex());
        ui->searchJourneyResults->append(fahrplan->timetable()->data(index, Timetable::CurrentStation).toString());
        ui->searchJourneyResults->append(fahrplan->timetable()->data(index, Timetable::Time).toTime().toString(QLocale().timeFormat(QLocale::ShortFormat)));
        ui->searchJourneyResults->append(fahrplan->timetable()->data(index, Timetable::DestinationStation).toString());
        ui->searchJourneyResults->append(fahrplan->timetable()->data(index, Timetable::TrainType).toString());
        ui->searchJourneyResults->append(fahrplan->timetable()->data(index, Timetable::Platform).toString());
        ui->searchJourneyResults->append(fahrplan->timetable()->data(index, Timetable::MiscInfo).toString());
        ui->searchJourneyResults->append("------------------------------");
    }
    QScrollBar *scrollbar = ui->searchJourneyResults->verticalScrollBar();
    scrollbar->setValue(scrollbar->minimum());
}

void MainWindow::stationsResult()
{
    ui->searchJourneyResults->clear();
    if (fahrplan->stationSearchResults()->count() > 0) {
        ui->stationsList->setCurrentIndex(fahrplan->stationSearchResults()->index(0));
    } else {
        ui->searchJourneyResults->append("No Results");
    }
}

void MainWindow::searchJourneyClicked()
{
    ui->searchJourneyResults->clear();
    ui->searchJourneyResults->append("Searching...");
    fahrplan->setDateTime(QDateTime::currentDateTime());
    fahrplan->setMode(Fahrplan::DepartureMode);
    fahrplan->setTrainrestriction(ui->trainRestrictions->currentIndex());
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
    QScrollBar *scrollbar = ui->searchJourneyResults->verticalScrollBar();
    scrollbar->setValue(scrollbar->minimum());
}

void MainWindow::getJourneyDetailsClicked()
{
    ui->getJourneyDetailsResults->clear();
    ui->getJourneyDetailsResults->append("Loading...");
    fahrplan->parser()->getJourneyDetails(ui->journeyResultItemIds->currentText());
}

void MainWindow::selectStationClicked()
{
    if (ui->stationsList->currentIndex().isValid())
        fahrplan->stationSearchResults()
                ->selectStation(ui->stationType->itemData(ui->stationType->currentIndex()).toInt(),
                                ui->stationsList->currentIndex().row());
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
    QScrollBar *scrollbar = ui->getJourneyDetailsResults->verticalScrollBar();
    scrollbar->setValue(scrollbar->minimum());
}
