#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "testkdtree.h"
#include <QFileDialog>

void runTests()
{
    TestKDTree tests;
    QTest::qExec(&tests);
}

using namespace Wrap;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    runTests();
    QMenu *menuEdit = menuBar()->addMenu("Edit");
    menuEdit->addAction("Load geometry",this,&MainWindow::loadGeometryFromFile,QKeySequence::Open);
    menuEdit->addAction("Fit to view",this,&MainWindow::fitToView,QKeySequence(Qt::Key_F));
    menuEdit->addAction("Neares dot test",this,&MainWindow::nearestFinderTriggered,QKeySequence(Qt::Key_H));
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

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(viewport);
    centralWidget()->setLayout(layout);
    spinboxX = new QDoubleSpinBox();
    spinboxY = new QDoubleSpinBox();
    spinboxZ = new QDoubleSpinBox();

    spinboxX->setMinimum(-100);
    spinboxX->setMaximum(100);
    spinboxY->setMinimum(-100);
    spinboxY->setMaximum(100);
    spinboxZ->setMinimum(-100);
    spinboxZ->setMaximum(100);

    spinboxX->setSingleStep(0.5);
    spinboxY->setSingleStep(0.5);
    spinboxZ->setSingleStep(0.5);

    QObject::connect(
                spinboxX, SIGNAL(valueChanged(double )),
                this, SLOT(valueChanged(double )));
    QObject::connect(
                spinboxY, SIGNAL(valueChanged(double )),
                this, SLOT(valueChanged(double )));
    QObject::connect(
                spinboxZ, SIGNAL(valueChanged(double )),
                this, SLOT(valueChanged(double )));

    layout->addWidget(spinboxX);
    layout->addWidget(spinboxY);
    layout->addWidget(spinboxZ);

    glGrid = new WGLGrid(20,1,WGLGrid::Axis_X,WGLGrid::Axis_Z,10);
    glGrid->setColor(QColor(60,60,60));
    viewport->addObject(glGrid);
    glMaterialSurface = new WGLMaterialSurface();
    glMaterialWireframe = new WGLMaterialWireframe();
}

void MainWindow::deinitializeViewport()
{
    viewport->removeObject(glGrid);
    delete glGrid;
    delete glMaterialSurface;
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
    geom = new GeometryStack2::GeomStackTriangulated(fileName,errorMsg);
    QVector<int> indexArray = KdTree::buildIndexArray(geom->v);
    tree = KdTree::kdTreeBuild(geom->v, indexArray);
    if (!errorMsg.isNull())
        qFatal(QString("Can not load file: %1").arg(fileName).toUtf8().constData());
    glData = new WGLDataGeomStackTriangulated(*geom);
    glRenderer = new WGLObjectRenderer(glData,glMaterialSurface,WGLObjectRenderer::CullFace_Back,true);
    glRenderer->addRenderPass(glMaterialWireframe,WGLObjectRenderer::CullFace_None,false);
    viewport->addObject(glRenderer);
    fitToView();
}

void MainWindow::fitToView()
{
    WBoundingBox boundingBox(QVector3D(-1,-1,-1) * 5, QVector3D(1,1,1) * 5);
    if (hasGeometry())
        boundingBox = WBoundingBox(geom->v);
    camera->fitToView(boundingBox,viewport->size());
    viewport->updateGL();
}

void MainWindow::nearestFinderTriggered()
{
    PointFinderNaive *finderNaive = new PointFinderNaive(geom->v);
    float timeNaive = MainWindow::nearestFinderTest(geom->v, finderNaive);

    PointFinderKDTree *finderKDTree = new PointFinderKDTree(tree);
    float timeKDTree = MainWindow::nearestFinderTest(geom->v, finderKDTree);

    QString message = "KDTree search: " + QString::number(timeKDTree)
            + "\nNaive search: " + QString::number(timeNaive);
    QMessageBox::information(this, "Nearest test", message);
}

void MainWindow::valueChanged(double a)
{
    if (tree == NULL)
        return;
    if (hasDots())
        clearDots();
    QVector3D point((float)spinboxX->value(), (float)spinboxY->value(), (float)spinboxZ->value());
    PointFinderKDTree *finderKDTree = new PointFinderKDTree(tree);
    int bestIndex = finderKDTree->findNearestPoint(point);
    QVector3D best = geom->v[bestIndex];
    MainWindow::createDots(point, best);
    viewport->updateGL();
}

float MainWindow::nearestFinderTest(QVector<QVector3D> &mesh, PointFinder *finder)
{
    QElapsedTimer timer;
    timer.start();
    int best = -1;
    for (int index = 0; index < mesh.size(); index++)
        best = finder->findNearestPoint(mesh[index]);

    return timer.elapsed()/1000.0;
}

void MainWindow::clearGeometry()
{
    Q_ASSERT(hasGeometry());
    viewport->removeObject(glRenderer);
    delete glRenderer;
    glRenderer = nullptr;
    delete glData;
    glData = nullptr;
    delete geom;
    geom = nullptr;
}

bool MainWindow::hasGeometry() const
{
    return geom != nullptr;
}

void MainWindow::createDots(QVector3D &point, QVector3D nearestPoint)
{
    QVector<QVector3D> dots = { point, nearestPoint };
    glDots = new WGLDots(dots);
    glDots->setPointSize(4);
    glDots->setColor(QColor(255,0,255));
    viewport->addObject(glDots,WViewport::Overlay);
}

void MainWindow::clearDots()
{
    viewport->removeObject(glDots,WViewport::Overlay);
    delete glDots;
    glDots = nullptr;
}

bool MainWindow::hasDots() const
{
    return glDots != nullptr;
}

