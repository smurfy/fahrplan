#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../../parser/parser_hafasxml.h"

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
    void findStationsByNameClicked();
    void findStationsByCoordinatesClicked();
    void stationsResult(StationsResultList *result);

private:
    Ui::MainWindow *ui;
    ParserAbstract *backendParser;
};

#endif // MAINWINDOW_H
