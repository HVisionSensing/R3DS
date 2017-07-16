#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include "objloader.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public:
    ObjLoader *objLoader = NULL;

private slots:
    void on_actionOpen_triggered();

    void on_actionSave_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
