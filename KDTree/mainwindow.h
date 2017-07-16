#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "wcontextnavigation.h"
#include "wgldatageomstacktriangulated.h"
#include "wglmaterialsurface.h"
#include "wglmaterialwireframe.h"
#include "wglobjectrenderer.h"
#include "wglgrid.h"
#include "wgldots.h"
#include "kdtree.h"
#include "pointfinder.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
    Wrap::WCamera3D *camera = nullptr;
    Wrap::WViewport *viewport = nullptr;
    Wrap::WContextNavigation *contextNavigation = nullptr;
    Wrap::WGLMaterialSurface *glMaterialSurface = nullptr;
    Wrap::WGLMaterialWireframe *glMaterialWireframe = nullptr;
    Wrap::WGLGrid *glGrid = nullptr;
    GeometryStack2::GeomStackTriangulated *geom = nullptr;
    Wrap::WGLDataGeomStackTriangulated *glData = nullptr;
    Wrap::WGLObjectRenderer *glRenderer = nullptr;
    Wrap::WGLDots *glDots = nullptr;
private:
    Node *tree = nullptr;
    QDoubleSpinBox *spinboxX = NULL;
    QDoubleSpinBox *spinboxY = NULL;
    QDoubleSpinBox *spinboxZ = NULL;
protected:
    void initializeViewport();
    void deinitializeViewport();
    void clearGeometry();
    bool hasGeometry() const;
    void createDots(QVector3D &point, QVector3D nearestPoint);
    void clearDots();
protected slots:
    bool hasDots() const;
    void loadGeometryFromFile();
    void fitToView();
    void nearestFinderTriggered();
    void valueChanged(double a);
    float nearestFinderTest(QVector<QVector3D> &mesh, PointFinder *finder);
};

#endif // MAINWINDOW_H
