#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "objloader.h"
#include "objloadertests.h"
#include "kdtree.h"
#include "pointfinder.h"

void runTests()
{
    ObjLoaderTests tests;
    QTest::qExec(&tests);
}



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    runTests();
}



MainWindow::~MainWindow()
{
    delete ui;
}



float MainWindow::nearestSearchTest(QVector<QVector3D> mesh1, PointFinder &finder)
{
    float startClock = clock();
    QVector3D best;
    for (int indexPoint = 0; indexPoint < mesh1.size(); indexPoint++){
        best = finder.findNearestPoint(mesh1.at(indexPoint));
    }
    float endClock = clock();
    return (endClock-startClock)/CLK_TCK;
}



void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open file", "", "OBJ-file (*.obj)");
    if (fileName.isNull())
        return;

    objLoader = new ObjLoader;

    QTextStream fileNameStream(&fileName);

    if (!(objLoader->loadFile(fileNameStream))){
        QMessageBox::warning(this, "exception", "File is damaged");
        return;
    }

    if (objLoader->getSizeVertices())
        objLoader->indexVertices = ObjLoader::triangulation(objLoader->indexVertices, objLoader->polygonStart);
    if (objLoader->getSizeTextures())
        objLoader->indexTextures = ObjLoader::triangulation(objLoader->indexTextures, objLoader->polygonStart);
    if (objLoader->getSizeNormals())
        objLoader->indexNormals = ObjLoader::triangulation(objLoader->indexNormals, objLoader->polygonStart);
}



void MainWindow::on_actionSave_triggered()
{

    if (objLoader == NULL){
        QMessageBox::warning(this, "exception", "nothing to save");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, "Save File", "", "*.obj");
    objLoader->saveFile(fileName, objLoader->verticesList, objLoader->normalsList, objLoader->texturesList,
                        objLoader->indexVertices, objLoader->indexNormals, objLoader->indexTextures);

}



void MainWindow::on_actiontest_triggered()
{
    if (objLoader == NULL){
        QMessageBox::warning(this, "exception", "not mesh");
        return;
    }

    PointFinderNaive finderNaive(objLoader->verticesList);
    float timeNaive = MainWindow::nearestSearchTest(objLoader->verticesList, finderNaive);

    Node *tree = KdTree::kdTreeBuild(objLoader->verticesList);
    PointFinderKDTree finderKDTree(tree);
    float timeKDTree = MainWindow::nearestSearchTest(objLoader->verticesList, finderKDTree);

    QString message = "Without KDTree: " + QString::number(timeNaive) + "\nWith KDTree: " + QString::number(timeKDTree);
    QMessageBox::information(this, "Nearest test", message);
}
