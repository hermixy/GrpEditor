#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "palletewindow.h"
#include "data.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void loadData();
    void loadWpe();
    void loadMapping();
    void loadRemapping();
    void loadColorCycling();

public slots:
    void loadGrp();
    void saveGrp();

private:
    Data *data;
    PalleteWindow *palleteWindow;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
