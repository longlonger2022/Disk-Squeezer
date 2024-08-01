#include "DiskSqueezer_v2Beta.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DiskSqueezer_v2Beta w;
    w.show();
    return a.exec();
}
