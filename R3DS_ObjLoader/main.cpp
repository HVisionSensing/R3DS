#include "objloader.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ObjLoader w;
    w.show();

    return a.exec();
}
