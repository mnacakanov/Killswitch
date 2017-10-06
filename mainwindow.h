#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QtWebChannel/QWebChannel>
#include <QMessageBox>
#include <QTest>
#include "wmiapp.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

// Members
private:
    Ui::MainWindow *ui;
    QWebEngineView* webview;
    WMIapp* wmiApp;
    QVector<QAxObject *> ipRouteTable;
    QVector<QAxObject *> adapterList;
    QVector<QAxObject *> adapterSetting;
    QWebChannel* webChannel;

// Methods
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Q_INVOKABLE void checkboxClicked(bool state);
    bool checkNetworkConditions(void);

private:
    void callUpdateCheckbox(bool state);
    void updateHeader(bool state);
    void enableKillSwitch(void);
    void disableKillSwitch(void);
};

#endif // MAINWINDOW_H
