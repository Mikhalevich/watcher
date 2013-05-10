#include <QtWidgets/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName(QLatin1String("watcher"));
    a.setApplicationVersion(QLatin1String("1.0"));

    MainWindow w;
    w.show();

    return a.exec();
}
