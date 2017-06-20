#include <objloader.h>
#include <QCoreApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Obj_loader& objLoaderInstance = Obj_loader::Instance();

    objLoaderInstance.loadObjFile("D:\\test.obj");

    objLoaderInstance.listShow(objLoaderInstance.verticesList);
    objLoaderInstance.listShow(objLoaderInstance.normalsList);
    objLoaderInstance.listShow(objLoaderInstance.texturesList);



    return a.exec();
}
