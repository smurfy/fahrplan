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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setAttribute(Qt::WA_Maemo5AutoOrientation, true);
    connect(QApplication::desktop(), SIGNAL(resized(int)), this, SLOT(orientationChanged()));

    setupUi();
    orientationChanged();

    fahrplan = new Fahrplan();
    connect(fahrplan, SIGNAL(parserStationsResult(StationsResultList*)), this, SLOT(stationsResult(StationsResultList*)));
    connect(fahrplan, SIGNAL(parserErrorOccured(QString)), this, SLOT(errorOccured(QString)));
    connect(fahrplan, SIGNAL(parserJourneyResult(JourneyResultList*)), this, SLOT(journeyResult(JourneyResultList*)));
    connect(fahrplan, SIGNAL(parserJourneyDetailsResult(JourneyDetailResultList*)), this, SLOT(journeyDetailResult(JourneyDetailResultList*)));
    connect(fahrplan, SIGNAL(parserChanged(QString, int)), this, SLOT(parserChanged(QString, int)));

    fahrplan->parser();
}

void MainWindow::setupUi()
{
    //Setting Up The GUI
    setAttribute(Qt::WA_Maemo5StackedWindow);
    setWindowTitle("Fahrplan2");

    QWidget *mainWidget = new QWidget;
    setCentralWidget(mainWidget);
    QGridLayout *layout = new QGridLayout();
    mainWidget->setLayout(layout);

    //Departure and Arrival Buttons
    QGridLayout *stationsLayout = new QGridLayout();
    QWidget *stationsContainerWidget = new QWidget;

    butDeparture = new QValueButton(tr("Departure station"), tr("please select"));
    butArrival = new QValueButton(tr("Arrival station"), tr("please select"));
    butSwap = new QPushButton(QChar(8597));

    stationsLayout->setMargin(0);
    stationsLayout->setSpacing(10);
    stationsLayout->addWidget(butDeparture, 0, 0);
    stationsLayout->addWidget(butArrival, 1, 0);
    stationsLayout->addWidget(butSwap, 0, 1, 0, 1);
    stationsContainerWidget->setLayout(stationsLayout);

    layout->addWidget(stationsContainerWidget, 0, 0, 1, 0);

    //Date and Time Buttons
    butDateSelect  = new QMaemo5ValueButton(tr("Date"));
    butDateSelect->setPickSelector(new QMaemo5DatePickSelector());
    butDateSelect->setValueLayout(QMaemo5ValueButton::ValueUnderTextCentered);

    butTimeSelect  = new QMaemo5ValueButton(tr("Time"));
    butTimeSelect->setPickSelector(new QMaemo5TimePickSelector());
    butTimeSelect->setValueLayout(QMaemo5ValueButton::ValueUnderTextCentered);

    //Departure/Arrival Selector Button
    butTimeMode = new QValueButton(tr("Use selected date/time as"), tr("Departure time"));
    butTimeModeUserData *mode = new butTimeModeUserData();
    mode->mode = 1;
    butTimeMode->setUserData(0, mode);
    connect(butTimeMode, SIGNAL(clicked(bool)), this, SLOT(butTimeModeClicked()));

    //Restrict Train Types
    butTrainRestrictions = new QValueButton(tr("Restrict train types"), "");

    //Search Button
    butSearch = new QPushButton();
    butSearch->setText(tr("Search"));
    connect(butSearch, SIGNAL(clicked(bool)), this, SLOT(butSearchClicked()));
}

void MainWindow::orientationChanged()
{
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    QGridLayout *layout = (QGridLayout*)this->centralWidget()->layout();

    if (screenGeometry.width() > screenGeometry.height()) //landscape
    {
        layout->addWidget(butDateSelect, 1, 0);
        layout->addWidget(butTimeSelect, 1, 1);
        layout->addWidget(butTimeMode, 2, 0);
        layout->addWidget(butTrainRestrictions, 2, 1);
        layout->addWidget(butSearch, 3, 0, 1, 0);

        butDateSelect->setFixedHeight(78);
        butTimeSelect->setFixedHeight(78);
        butTimeMode->setFixedHeight(78);
        butTrainRestrictions->setFixedHeight(78);
        butDeparture->setFixedHeight(78);
        butArrival->setFixedHeight(78);
        butSwap->setFixedHeight(160);
        butSwap->setFixedWidth(100);
        butSearch->setFixedHeight(78);

    } else { //portrait
        layout->addWidget(butDateSelect, 1, 0, 1, 0);
        layout->addWidget(butTimeSelect, 2, 0, 1, 0);
        layout->addWidget(butTimeMode, 3, 0, 1, 0);
        layout->addWidget(butTrainRestrictions, 4, 0, 1, 0);
        layout->addWidget(butSearch, 5, 0, 1, 0);

        butDateSelect->setFixedHeight(98);
        butTimeSelect->setFixedHeight(98);
        butTimeMode->setFixedHeight(98);
        butTrainRestrictions->setFixedHeight(98);
        butDeparture->setFixedHeight(98);
        butArrival->setFixedHeight(98);
        butSwap->setFixedHeight(200);
        butSwap->setFixedWidth(100);
        butSearch->setFixedHeight(98);
    }
}

void MainWindow::butSearchClicked()
{

}

void MainWindow::stationsResult(StationsResultList *result)
{

}
void MainWindow::journeyResult(JourneyResultList *result)
{

}
void MainWindow::journeyDetailResult(JourneyDetailResultList *result)
{

}
void MainWindow::errorOccured(QString msg)
{
    QMaemo5InformationBox::information(this, msg, QMaemo5InformationBox::DefaultTimeout);
}

void MainWindow::parserChanged(QString name, int index)
{
    setWindowTitle("Fahrplan (" + name + ")");

    QStringList trainRestrictions = fahrplan->parser()->getTrainRestrictions();
    butTrainRestrictions->setEnabled(false);
    butTrainRestrictions->setDescription(tr("not supported"));
    if (trainRestrictions.count() > 0)
    {
        QStandardItemModel * trainrestrModel = new QStandardItemModel(0, 1);
        for (int i = 0; i < trainRestrictions.count(); i++)
        {
            QStandardItem *item = new QStandardItem(trainRestrictions[i]);
            item->setTextAlignment(Qt::AlignCenter);
            item->setEditable(false);
            trainrestrModel->appendRow(item);
        }
        butTrainRestrictions->setModel(trainrestrModel);
        butTrainRestrictions->setEnabled(true);
        butTrainRestrictions->setModelIndex(0);
    }
}

void MainWindow::butTimeModeClicked()
{
    QValueButton *button = (QValueButton *)sender();
    butTimeModeUserData *mode = (butTimeModeUserData*)button->userData(0);
    if (mode->mode == 0)
    {
        mode->mode = 1;
        butTimeMode->setDescription(tr("Departure time"));

    } else {
        mode->mode = 0;
        butTimeMode->setDescription(tr("Arrival time"));
    }
    button->setUserData(0,mode);
}
