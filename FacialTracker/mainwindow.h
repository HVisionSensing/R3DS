#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "QString"
#include "dataset.h"
#include "loader.h"
#include "facedetector.h"
#include "sdm.h"
#include "facedescriptor.h"
#include "pca.h"
#include "facialtrackertest.h"

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
    void on_horizontalSlider_sliderReleased();

    void on_horizontalSlider_2_sliderReleased();

    void on_horizontalSlider_3_sliderReleased();

    void on_horizontalSlider_4_sliderReleased();

    void on_horizontalSlider_5_sliderReleased();

    void on_horizontalSlider_valueChanged(int value);

    void on_horizontalSlider_2_valueChanged(int value);

    void on_horizontalSlider_3_valueChanged(int value);

    void on_horizontalSlider_4_valueChanged(int value);

    void on_horizontalSlider_5_valueChanged(int value);

private:

    void draw(int num, float value);

    Ui::MainWindow *ui;
    PCA *pca;
    VectorXf eigenVel;
    VectorXf newFace;
    MatrixXf eigenVec;
};

#endif // MAINWINDOW_H
