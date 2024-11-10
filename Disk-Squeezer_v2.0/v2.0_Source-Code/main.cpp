#include "DiskSqueezer_v20.h"
#include <QtWidgets/QApplication>
#include <QFontDatabase>
#include <QFont>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DiskSqueezer_v20 w;
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    w.show();
    return a.exec();
}
