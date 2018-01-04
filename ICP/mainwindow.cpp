#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "Eigen/Core"

using namespace Wrap;

void runTests()
{
    TestGradientDescent tests;
    QTest::qExec(&tests);
}



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    runTests();
    QMenu *menuEdit = menuBar()->addMenu("Edit");
    menuEdit->addAction("Load geometry",this,&MainWindow::loadGeometryFromFile,QKeySequence::Open);
    menuEdit->addAction("Fit to view",this,&MainWindow::fitToView,QKeySequence(Qt::Key_F));
    menuEdit->addAction("ICP",this,&MainWindow::icp,QKeySequence(Qt::Key_L));
    menuEdit->addAction("Rigid alignment",this,&MainWindow::rigidAlignment,QKeySequence(Qt::Key_I));
    menuEdit->addAction("Clear",this,&MainWindow::clearGeometry,QKeySequence(Qt::Key_J));

    initializeViewport();
}



MainWindow::~MainWindow()
{
    if (hasGeometry())
        clearGeometry();
    clearDots();
    deinitializeViewport();
    delete ui;
}



void MainWindow::initializeViewport()
{
    camera = new WCamera3D(25,QVector3D(0,0,3),QVector3D(),WCamera3D::Perspective,0.01,1000);
    viewport = new WViewport(camera);
    viewport->makeCurrent();
    contextNavigation = new WContextNavigation(viewport,camera);
    contextNavigation->setRotationMode(WContextNavigation::Standard);
    setCentralWidget(viewport);
    glGrid = new WGLGrid(20,1,WGLGrid::Axis_X,WGLGrid::Axis_Z,10);
    glGrid->setColor(QColor(60,60,60));
    viewport->addObject(glGrid);
    glMaterialSurfaceOne = new WGLMaterialSurface();
    glMaterialSurfaceTwo = new WGLMaterialSurface();
    glMaterialWireframe = new WGLMaterialWireframe();

    QString errorMsg;
    geomFirst = new GeometryStack2::GeomStackTriangulated("../ICP/Models/monkey.obj",errorMsg);
    if (!errorMsg.isNull())
        qFatal(QString("Can not load file: %1").arg("../ICP/Models/monkey.obj").toUtf8().constData());
    glDataFirst = new WGLDataGeomStackTriangulated(*geomFirst);
    glMaterialSurfaceOne->setColor(QColor(255,0,0));
    glRendererFirst = new WGLObjectRenderer(glDataFirst,glMaterialSurfaceOne,WGLObjectRenderer::CullFace_Back,true);
    glRendererFirst->addRenderPass(glMaterialWireframe,WGLObjectRenderer::CullFace_None,false);
    viewport->addObject(glRendererFirst);

    geomSecond  = new GeometryStack2::GeomStackTriangulated("../ICP/Models/monkey2.obj",errorMsg);
    if (!errorMsg.isNull())
        qFatal(QString("Can not load file: %1").arg("../ICP/Models/monkey2.obj").toUtf8().constData());
    glDataSecond = new WGLDataGeomStackTriangulated(*geomSecond);
    glMaterialSurfaceTwo->setColor(QColor(0,255,0));
    glRendererSecond = new WGLObjectRenderer(glDataSecond,glMaterialSurfaceTwo,WGLObjectRenderer::CullFace_Back,true);
    glRendererSecond->addRenderPass(glMaterialWireframe,WGLObjectRenderer::CullFace_None,false);
    QVector<int> indexArray = KdTree::buildIndexArray(geomSecond->v);
    tree = KdTree::kdTreeBuild(geomSecond->v, indexArray);
    viewport->addObject(glRendererSecond);

    /*QVector<QVector3D> vertices = { QVector3D(0,0,0), QVector3D(1,0,0)};
    glLines = new WGLLines(vertices);
    glLines->setColor(Qt::yellow);
    glLines->setLineWidth(1);
    viewport->addObject(glLines,WViewport::Overlay);*/
}



void MainWindow::deinitializeViewport()
{
    viewport->removeObject(glGrid);
    delete glGrid;
    delete glMaterialSurfaceOne;
    delete glMaterialSurfaceTwo;
    delete glMaterialWireframe;
    delete contextNavigation;
    delete viewport;
    delete camera;
}



void MainWindow::loadGeometryFromFile()
{
    if (hasGeometry())
        clearGeometry();
    const QString fileName = QFileDialog::getOpenFileName(this,"Load geometry",QString(),"OBJ-file (*.obj)");
    if (fileName.isNull())
        return;
    QString errorMsg;
    if (geomFirst == nullptr){
        geomFirst = new GeometryStack2::GeomStackTriangulated(fileName,errorMsg);
        if (!errorMsg.isNull())
            qFatal(QString("Can not load file: %1").arg(fileName).toUtf8().constData());
        glDataFirst = new WGLDataGeomStackTriangulated(*geomFirst);
        glRendererFirst = new WGLObjectRenderer(glDataFirst,glMaterialSurfaceOne,WGLObjectRenderer::CullFace_Back,true);
        glRendererFirst->addRenderPass(glMaterialWireframe,WGLObjectRenderer::CullFace_None,false);
        viewport->addObject(glRendererFirst);
    } else if (geomSecond == nullptr){
        geomSecond  = new GeometryStack2::GeomStackTriangulated(fileName,errorMsg);
        if (!errorMsg.isNull())
            qFatal(QString("Can not load file: %1").arg(fileName).toUtf8().constData());
        glDataSecond = new WGLDataGeomStackTriangulated(*geomSecond);
        glRendererSecond = new WGLObjectRenderer(glDataSecond,glMaterialSurfaceTwo,WGLObjectRenderer::CullFace_Back,true);
        glRendererSecond->addRenderPass(glMaterialWireframe,WGLObjectRenderer::CullFace_None,false);
        QVector<int> indexArray = KdTree::buildIndexArray(geomSecond->v);
        tree = KdTree::kdTreeBuild(geomSecond->v, indexArray);
        viewport->addObject(glRendererSecond);
    }
    fitToView();
}



void MainWindow::fitToView()
{
    WBoundingBox boundingBox(QVector3D(-1,-1,-1) * 5, QVector3D(1,1,1) * 5);
    if (hasGeometry())
        boundingBox = WBoundingBox(geomFirst->v);
    camera->fitToView(boundingBox,viewport->size());
    viewport->updateGL();
}



void MainWindow::rigidAlignment()
{
    if(!hasGeometry())
        return;
    UpdateFunctorViewPort *update = new UpdateFunctorViewPort(geomFirst, glDataFirst, glRendererFirst, viewport, glMaterialSurfaceOne, glMaterialWireframe);
    ProblemVector x(0,0,0,0,Vector3<float>(1,0,0));
    RigidAlignemntFirst<ErrorFunctionQua> *rigidAlignment = new RigidAlignemntFirst<ErrorFunctionQua>(geomFirst->v, geomSecond->v, ErrorFunctionQua(), update);
    x = rigidAlignment->rigidAlignment(x, true);
    //x = GradientDescent::gradientDescent(geomFirst->v, geomSecond->v, ErrorFunctionQua(), x, true, update);
    //update->update(x.angle, x.shiftX, x.shiftY, x.shiftZ, x.axis, geomFirst->v);
}



void MainWindow::clearGeometry()
{
    if (geomFirst != nullptr){
        viewport->removeObject(glRendererFirst);
        delete glRendererFirst;
        glRendererFirst = nullptr;
        delete glDataFirst;
        glDataFirst = nullptr;
        delete geomFirst;
        geomFirst = nullptr;
    }
    if (geomSecond != nullptr){
        viewport->removeObject(glRendererSecond);
        delete glRendererSecond;
        glRendererSecond = nullptr;
        delete glDataSecond;
        glDataSecond = nullptr;
        delete geomSecond;
        geomSecond = nullptr;
    }

    viewport->update();
}



bool MainWindow::hasGeometry() const
{
    return (geomFirst != nullptr) && (geomSecond != nullptr);
}



void MainWindow::createDots()
{
    QVector<QVector3D> dots = { QVector3D(0,0,0),QVector3D(1,0,0),QVector3D(0,1,0),QVector3D(0,0,1) };
    glDots = new WGLDots(dots);
    glDots->setPointSize(3);
    glDots->setColor(QColor(255,0,255));
    viewport->addObject(glDots,WViewport::Overlay);
}



void MainWindow::clearDots()
{
    viewport->removeObject(glDots,WViewport::Overlay);
    delete glDots;
    glDots = nullptr;
}



void MainWindow::icp()
{
    if(!hasGeometry())
        return;
    UpdateFunctorViewPort *update = new UpdateFunctorViewPort(geomFirst, glDataFirst, glRendererFirst, viewport, glMaterialSurfaceOne, glMaterialWireframe);
    PointFinderKDTree *finder = new PointFinderKDTree(tree);
    RigidAlignemntFirst<ErrorFunctionQua> *rigidAlignment = new RigidAlignemntFirst<ErrorFunctionQua>(geomFirst->v, geomSecond->v, ErrorFunctionQua(), update);

    QVector<QVector3D> vertices;
    for (int index = 0; index < geomFirst->v.size(); index++){
        vertices.append(geomFirst->v[index]);
        vertices.append(geomSecond->v[index]);
    }

    glLines = new WGLLines(vertices);
    glLines->setColor(Qt::yellow);
    glLines->setLineWidth(1);
    viewport->addObject(glLines,WViewport::Overlay);

    ProblemVector x = ICP::icp(finder, rigidAlignment, glLines, viewport);
    //update->update(x.angle, x.shiftX, x.shiftY, x.shiftZ, x.axis, geomFirst->v);
    //viewport->update();
    //QApplication::processEvents();
    //ProblemVector x = GradientDescent::gradientDescent(geomFirst->v, geomSecond->v, ErrorFunctionRod(), update, tree);
}

