//#include "wmiapp.h"
#include <QApplication>
#include <QString>
#include <QDebug>
#include <QTest>
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

//    Controller* controller = new Controller();

//    controller->mainCycle();
//    WMIapp WMIapplication;
//    WMIapplication.initNetworkAdapterInterfacing();
//    QVector<QAxObject*> networkAdapters;
//        QTest::qWait(1000);
//    int numInterfaces = WMIapplication.getConnectedNetworkAdapterObj(networkAdapters);
//    qDebug() << "Got " << networkAdapters.size() << " interfaces";
//    for(int i = 0; i < networkAdapters.size(); i++)
//    {
//        WMIapplication.executeMethod(networkAdapters[i], "Disable");
//    }
//    QTest::qWait(10000);
//    for(int i = 0; i < networkAdapters.size(); i++)
//    {
//        WMIapplication.executeMethod(networkAdapters[i], "Enable");
//    }


}
