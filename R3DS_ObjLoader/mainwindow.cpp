#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "objloader.h"
#include "objloadertests.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void runTests()
{
    ObjLoaderTests tests;
    QTest::qExec(&tests);
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open file", "", "*.obj");

    if (objLoader.verticesList.size())
        objLoader.verticesList.remove(0,  objLoader.verticesList.size());
    if (objLoader.normalsList.size())
        objLoader.normalsList.remove(0,  objLoader.normalsList.size());
    if (objLoader.texturesList.size())
        objLoader.texturesList.remove(0,  objLoader.texturesList.size());

    if (objLoader.indexVertices.size())
        objLoader.indexVertices.remove(0,  objLoader.indexVertices.size());
    if (objLoader.indexNormals.size())
        objLoader.indexNormals.remove(0,  objLoader.indexNormals.size());
    if (objLoader.indexTextures.size())
        objLoader.indexTextures.remove(0,  objLoader.indexTextures.size());
    if (objLoader.polygonStart.size())
        objLoader.polygonStart.remove(0,  objLoader.polygonStart.size());

    objLoader.loadObjFile(fileName);

    if (objLoader.verticesList.size()){
        objLoader.indexVertices = ObjLoader::triangulation(objLoader.indexVertices, objLoader.polygonStart);
    }
    if (objLoader.texturesList.size()){
        objLoader.indexTextures = ObjLoader::triangulation(objLoader.indexTextures, objLoader.polygonStart);
    }
    if (objLoader.normalsList.size()){
        objLoader.indexNormals = ObjLoader::triangulation(objLoader.indexNormals, objLoader.polygonStart);
    }

    runTests();
}

void MainWindow::on_actionSave_triggered()
{
    if (objLoader.verticesList.size()){
        QString fileName = QFileDialog::getSaveFileName(this, "Save File", "", "*.obj");
        objLoader.saveFile(fileName, objLoader.verticesList, objLoader.normalsList, objLoader.texturesList, objLoader.indexVertices, objLoader.indexNormals, objLoader.indexTextures);
    }
    else {
        QMessageBox::warning(this, "exception", "ERROR");
    }
}
