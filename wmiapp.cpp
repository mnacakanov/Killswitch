#include "wmiapp.h"


WMIapp::WMIapp()
{
    CoInitialize(0);
}
int WMIapp::initNetworkAdapterInterfacing(void)
{
    objIWbemLocator = new QAxObject("WbemScripting.SWbemLocator");
    if (!objIWbemLocator) {
      qDebug() << "WbemScripting.SWbemLocator not created";
      return -1;
    }
    objWMIService = objIWbemLocator->querySubObject(
            "ConnectServer(QString&,QString&)",
            QString("."), QString("root\\cimv2"));
    if (!objWMIService) {
      qDebug() << "WMIService not created";
      delete objIWbemLocator;
      return -1;
    }
//    qDebug() << objWMIService->generateDocumentation();
    connect(objWMIService, SIGNAL(exception(int,QString,QString,QString)), this, SLOT(outputLogError(int,QString,QString,QString)));
    return 0;
}
int WMIapp::getObjByQuarry(QVector<QAxObject *> &networkAdapters, QString querry)
{

  int numAdapters = 0;
//  QString query = QString("Select * from Win32_NetworkAdapter "
//                          "WHERE NetConnectionStatus = 2");
  QString query = QString(querry);
  QAxObject *objInterList = objWMIService->querySubObject(
          "ExecQuery(QString&))", query);
  if (!objInterList) {
    qDebug() << "Answer from ExecQuery is null";
    delete objIWbemLocator;
    numAdapters = -1;
    return numAdapters;
  }
  numAdapters = objInterList->dynamicCall("Count").toInt();
  if (numAdapters == 0) {
    qDebug() << "No matches";
    delete objIWbemLocator;
    return numAdapters;
  }

  QAxObject *enum1 = objInterList->querySubObject("_NewEnum");
  if (!enum1) {
    qDebug() << "Query _NewEnum failed";
    delete objIWbemLocator;
    numAdapters = -1;
    return numAdapters;
  }
  IEnumVARIANT* enumInterface = 0;
  enum1->queryInterface(IID_IEnumVARIANT, (void**)&enumInterface);
  if (!enumInterface) {
    qDebug() << "Query interface failed";
    delete objIWbemLocator;
    numAdapters = -1;
    return numAdapters;
  }
  enumInterface->Reset();
  numAdapters = objInterList->dynamicCall("Count").toInt();
  QAxObject *item = NULL;
  for (int i = 0; i < numAdapters; i++) {
    VARIANT *theItem = (VARIANT*)malloc(sizeof(VARIANT));
    if (enumInterface->Next(1,theItem,NULL) == S_FALSE) {
      qDebug() << "enum next failed";
      delete theItem;
      delete objIWbemLocator;
      numAdapters = -1;
      return numAdapters;
    }
    item = new QAxObject((IUnknown*)theItem->punkVal);
    if (!item) {
      qDebug() << "getting result item failed";
      delete theItem;
      delete objIWbemLocator;
      numAdapters = 0;
      return numAdapters;
    }
    networkAdapters.append(item);
    delete theItem;
  }

  enumInterface->Release();
  return numAdapters;
}
int WMIapp::getIPRouteTableObj(QVector<QAxObject *> &table)
{

  int numAdapters = 0;
  QString query = QString("Select * from Win32_IP4RouteTable ");
  QAxObject *objInterList = objWMIService->querySubObject(
          "ExecQuery(QString&))", query);
  if (!objInterList) {
    qDebug() << "Answer from ExecQuery is null";
    delete objIWbemLocator;
    numAdapters = -1;
    return numAdapters;
  }
  numAdapters = objInterList->dynamicCall("Count").toInt();
  if (numAdapters == 0) {
    qDebug() << "No adapters found";
    delete objIWbemLocator;
    return numAdapters;
  }
  qDebug() << numAdapters;
  QAxObject *enum1 = objInterList->querySubObject("_NewEnum");
  if (!enum1) {
    qDebug() << "Query _NewEnum failed";
    delete objIWbemLocator;
    numAdapters = -1;
    return numAdapters;
  }
  IEnumVARIANT* enumInterface = 0;
  enum1->queryInterface(IID_IEnumVARIANT, (void**)&enumInterface);
  if (!enumInterface) {
    qDebug() << "Query interface failed";
    delete objIWbemLocator;
    numAdapters = -1;
    return numAdapters;
  }
  enumInterface->Reset();
  numAdapters = objInterList->dynamicCall("Count").toInt();
  QAxObject *item = NULL;
  for (int i = 0; i < numAdapters; i++) {
    VARIANT *theItem = (VARIANT*)malloc(sizeof(VARIANT));
    if (enumInterface->Next(1,theItem,NULL) == S_FALSE) {
      qDebug() << "enum next failed";
      delete theItem;
      delete objIWbemLocator;
      numAdapters = -1;
      return numAdapters;
    }
    item = new QAxObject((IUnknown*)theItem->punkVal);
    if (!item) {
      qDebug() << "getting result item failed";
      delete theItem;
      delete objIWbemLocator;
      numAdapters = 0;
      return numAdapters;
    }
    table.append(item);
    delete theItem;
  }

  enumInterface->Release();
  return numAdapters;
}

QVariant WMIapp::getProperty(QAxObject* item, const char *propertyName)
{
    return item->dynamicCall(propertyName);
}

void WMIapp::executeMethod(QAxObject* item, const char* methodName)
{
    QAxObject* itemLocation =  item->querySubObject("Path_");
    objWMIService->querySubObject("ExecMethod(QString&,QString&)",
                                  itemLocation->dynamicCall("Relpath"),
                                  methodName);
}

QAxObject* WMIapp::deleteItem(QAxObject* item)
{
    QAxObject* itemLocation =  item->querySubObject("Path_");
    QAxObject* result = objWMIService->querySubObject("Get(QString&)",
                                  itemLocation->dynamicCall("Relpath"));
    objWMIService->querySubObject("Delete(QString&)",
                                  itemLocation->dynamicCall("Relpath"));

    return result;
}
void WMIapp::putItem(QAxObject* item)
{
    objWMIService->querySubObject("Put(IDispatch*)",
                                  item->asVariant());
    return;
}

void WMIapp::outputLogError(int num, QString one, QString two, QString three)
{
    qDebug() << "Errno: " << num;
    qDebug() << one;
    qDebug() << two;
    qDebug() << three;
}
WMIapp::~WMIapp()
{
    delete objIWbemLocator;
    CoUninitialize();
}
QString WMIapp::convertProperty(QAxObject* pObject, const char* pName)
{
    return pObject->dynamicCall(pName).toString();
}
