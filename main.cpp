#include <QApplication>
#include <QString>
#include <QDebug>
#include "application.h"

QString convertProperty(QAxObject* pObject, const char* pName)
{
    return pObject->dynamicCall(pName).toString();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Application* app = new Application();
    if (app->checkNetworkConditions())
    {
        app->show();
        return a.exec();
    }
    else
    {
        return 0;
    }
}
