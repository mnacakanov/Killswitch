#ifndef WMIAPP_H
#define WMIAPP_H

#include <QAxObject>
#include <QUuid>
#include <qt_windows.h>
#include <QWidget>
#include <QDebug>
#include <QVector>

class WMIapp : public QWidget
{
    Q_OBJECT
private slots:
    void outputLogError(int num, QString one, QString two, QString three);
private:
    QAxObject *objIWbemLocator;
    QAxObject *objWMIService;
public:
    int getObjByQuarry(QVector<QAxObject *> &networkAdapters, QString querry);
    int getIPRouteTableObj(QVector<QAxObject *> &table);
    int initNetworkAdapterInterfacing(void);
    QVariant getProperty(QAxObject* item, const char* propertyName);
    void executeMethod(QAxObject* item, const char* methodName);
    QAxObject* deleteItem(QAxObject* item);
    void putItem(QAxObject* item);
    QString convertProperty(QAxObject* pObject, const char* pName);
    WMIapp();
    ~WMIapp();
signals:
    void putSmth(IDispatch*);
};

#endif // WMIAPP_H
