#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <testGradientDescent.h>

#include "updatefunctor.h"
#include "wcontextnavigation.h"
#include "errorfunctions.h"
#include "wgldatageomstacktriangulated.h"
#include "wglmaterialsurface.h"
#include "wglmaterialwireframe.h"
#include "wglobjectrenderer.h"
#include "wglgrid.h"
#include "wgldots.h"
#include "wgllines.h"
#include "kdtree.h"
#include "icp.h"
#include "rigidalignment.h"

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
    Wrap::WGLMaterialSurface *glMaterialSurfaceOne = nullptr;
    Wrap::WGLMaterialSurface *glMaterialSurfaceTwo = nullptr;
    Wrap::WGLMaterialWireframe *glMaterialWireframe = nullptr;
    Wrap::WGLGrid *glGrid = nullptr;
    GeometryStack2::GeomStackTriangulated *geomFirst = nullptr;
    GeometryStack2::GeomStackTriangulated *geomSecond = nullptr;
    Wrap::WGLDataGeomStackTriangulated *glDataFirst = nullptr;
    Wrap::WGLDataGeomStackTriangulated *glDataSecond = nullptr;
    Wrap::WGLObjectRenderer *glRendererFirst = nullptr;
    Wrap::WGLObjectRenderer *glRendererSecond = nullptr;
    Wrap::WGLDots *glDots = nullptr;
    Wrap::WGLLines *glLines = nullptr;
    Node *tree = nullptr;
protected:
    void initializeViewport();
    void deinitializeViewport();
    void clearGeometry();
    bool hasGeometry() const;
    void createDots();
    void clearDots();
    void icp();
protected slots:
    void loadGeometryFromFile();
    void fitToView();
    void rigidAlignment();
};

#endif // MAINWINDOW_H
