#include <objloader.h>
#include <QCoreApplication>
#include <QDebug>
#include "objloadertests.h"

void runTests()
{
    ObjLoaderTests tests;
    QTest::qExec(&tests);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ObjLoader objLoader;

    objLoader.loadObjFile("D:\\ProjectCode\\R3DS_ObjLoader\\test\\Test_Vert_Text_Norm.obj");

    objLoader.pointShow(objLoader.verticesList);
    objLoader.pointShow(objLoader.normalsList);
    objLoader.pointShow(objLoader.texturesList);
    objLoader.polygonShow();

    runTests();
}
