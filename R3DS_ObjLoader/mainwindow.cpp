#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "objloader.h"
#include "objloadertests.h"
#include "kdtree.h"

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

    /*float start = clock();
    Node *best = NULL;
    Node *tree = KdTree::kdTreeBuild(objLoader->verticesList);
    float start = clock();
    for (int pointIndex = 0; pointIndex < objLoader->verticesList.size(); pointIndex++){
        tree->nearestNeighborSearch(objLoader->verticesList.operator [](pointIndex), best);
    }
    float end = clock();
    float f = (end-start)/CLK_TCK;
    //float test1 = KdTree::testingKnn(objLoader->verticesList, objLoader->verticesList, KdTree::nearestNeighborSearchBasic);
    //float test2 = KdTree::testingKnn(objLoader->verticesList, objLoader->verticesList, KdTree::nearestNeighborSearchNotBasic);
    int a = 5+3;*/
}
