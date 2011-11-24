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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QStringListModel>
#include "../../fahrplan.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void getTimeTableForStationClicked();
    void findStationsByNameClicked();
    void findStationsByCoordinatesClicked();
    void stationsResult(StationsResultList *result);
    void journeyResult(JourneyResultList *result);
    void journeyDetailResult(JourneyDetailResultList *result);
    void searchJourneyClicked();
    void searchJourneyEarlierClicked();
    void searchJourneyLaterClicked();
    void parserCurrentIndexChanged(int index);
    void cancelRequestClicked();
    void getJourneyDetailsClicked();
    void errorOccured(QString msg);
    void parserChanged(QString name, int index);

private:
    Ui::MainWindow *ui;
    Fahrplan *fahrplan;
};

#endif // MAINWINDOW_H
