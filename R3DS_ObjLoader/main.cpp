#include <objloader.h>
#include <QCoreApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Obj_loader& objLoader = Obj_loader::Instance();

    objLoader.loadObjFile("D:\\Test_Vert_Text_Norm.obj");

    objLoader.listShow(objLoader.verticesList);
    objLoader.listShow(objLoader.normalsList);
    objLoader.listShow(objLoader.texturesList);
    objLoader.show();
    return a.exec();
}
