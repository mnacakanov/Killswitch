#ifndef APPLICATION_H
#define APPLICATION_H

#include <QWidget>
#include <QPushButton>
#include <QApplication>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QMessageBox>
#include "wmiapp.h"

QT_BEGIN_NAMESPACE
class QCheckBox;
class QPushButton;
class QLabel;
QT_END_NAMESPACE

class Application : public QWidget
{
    Q_OBJECT
    WMIapp WMIapplication;
    QVector<QAxObject*> networkAdapters;
    int numInterfaces;
    int widgetHeight;
    int widgetWidth;
public:
    Application();
    ~Application();
    bool checkNetworkConditions(void);
private slots:
    void killSwitchEngaged(void);

private:
    QPushButton *quitButton;
    QCheckBox *killSwitchCheckBox;
};

#endif // APPLICATION_H
