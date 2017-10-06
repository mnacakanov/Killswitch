#include "mainwindow.h"
#include <QApplication>
#include <QDesktopServices>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    if (w.checkNetworkConditions())
    {
        w.show();
        return a.exec();
    }
    else
    {
        return 0;
    }
}
