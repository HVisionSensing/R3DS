#include "markupwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MarkupWindow w;
    w.show();

    return a.exec();
}
