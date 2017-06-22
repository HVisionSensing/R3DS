#include <objloader.h>
#include <QCoreApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Obj_loader& objLoader = Obj_loader::Instance();

    objLoader.loadObjFile("D:\\test.obj");

    objLoader.pointShow(objLoader.verticesList);
    objLoader.pointShow(objLoader.normalsList);
    objLoader.pointShow(objLoader.texturesList);
    objLoader.polygonShow(objLoader.polygonStart);

    return a.exec();
}
