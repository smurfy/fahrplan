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
#include <QtMaemo5>
#include <QMaemo5InformationBox>
#include <qgtkstyle.h>
#include "qvaluebutton.h"
#include "../../fahrplan.h"


class butTimeModeUserData : public QObjectUserData
{
public:
    int mode;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
    public:
        explicit MainWindow(QWidget *parent = 0);

    private slots:
        void orientationChanged();
        void butTimeModeClicked();
        void butSearchClicked();
        void stationsResult(StationsResultList *result);
        void journeyResult(JourneyResultList *result);
        void journeyDetailResult(JourneyDetailResultList *result);
        void errorOccured(QString msg);
        void parserChanged(QString name, int index);

    private:
        Fahrplan *fahrplan;
        QValueButton *butDeparture;
        //QValueButton *butVia;
        QValueButton *butArrival;
        QPushButton *butSwap;
        QMaemo5ValueButton *butDateSelect;
        QMaemo5ValueButton *butTimeSelect;
        QValueButton *butTimeMode;
        QValueButton *butTrainRestrictions;
        QPushButton *butSearch;

        void setupUi();
};

#endif // MAINWINDOW_H
