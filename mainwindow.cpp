#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    wmiApp = new WMIapp();
    wmiApp->initNetworkAdapterInterfacing();

    ui->setupUi(this);
    webview = new QWebEngineView();
    ui->verticalLayout->addWidget(webview);
    webChannel = new QWebChannel();
    webChannel->registerObject("mainWindow", this);
    webview->page()->setWebChannel(webChannel);
    webview->page()->load(QUrl("qrc:/index.html"));
}

MainWindow::~MainWindow()
{
    webChannel->deregisterObject(this);
    delete webChannel;
    delete ui;
    delete wmiApp;
    delete webview;
}

void MainWindow::callUpdateCheckbox(bool state)
{
    QString call;
    if(state)
    {
        call = "updateCheckbox(true);";
    }
    else
    {
        call = "updateCheckbox(false);";
    }
    qDebug() << call;

    webview->page()->runJavaScript(call);
    return;
}

void MainWindow::updateHeader(bool state)
{
    if (state)
    {
        ui->label->setText("KillSwitch Engaged");
    }
    else
    {
        ui->label->setText("KillSwitch Disengaged");
    }
}

void MainWindow::checkboxClicked(bool state)
{
    ui->label->setText("Processing Request");
    QTest::qWait(1000);
    if (state)
    {
        enableKillSwitch();
    }
    else
    {
        disableKillSwitch();
    }
    updateHeader(state);
    callUpdateCheckbox(state);
}

void MainWindow::disableKillSwitch(void)
{
    adapterList.clear();
    wmiApp->getObjByQuarry(adapterList,
                        "Select * from Win32_NetworkAdapter "
                        "WHERE NetConnectionStatus = 2");
    adapterSetting.clear();
    QString adapterSettingQuarry;
    adapterSettingQuarry = QString("Select * from Win32_NetworkAdapterConfiguration ")
            + QString("WHERE Index = ")
            + wmiApp->convertProperty(*adapterList.begin(), "Index");
    wmiApp->getObjByQuarry(adapterSetting,
                           adapterSettingQuarry
                          );
    wmiApp->executeMethod(*adapterSetting.begin(), "ReleaseDHCPLease");
    wmiApp->executeMethod(*adapterSetting.begin(), "RenewDHCPLease");
}

void MainWindow::enableKillSwitch(void)
{
    ipRouteTable.clear();
    wmiApp->getObjByQuarry(ipRouteTable, "Select * from Win32_IP4RouteTable ");

    for(int i = 0; i < ipRouteTable.size(); i++)
    {
        wmiApp->deleteItem(ipRouteTable[i]);
    }
}

bool MainWindow::checkNetworkConditions(void)
{
    bool isNetworkUp = true;
    int numInterfaces = 0;
    numInterfaces = wmiApp->getObjByQuarry(adapterList,
                        "Select * from Win32_NetworkAdapter "
                        "WHERE NetConnectionStatus = 2");
    if ( numInterfaces <= 0)
    {
        QMessageBox msgBox;
        msgBox.setText("No active internet connection.");
        msgBox.setInformativeText(tr("There's no active internet connection."
                                     "\nKillswitch only works with active internet connection."));
        msgBox.setStandardButtons(QMessageBox::Close);
        msgBox.exec();
        isNetworkUp = false;
    }
    return isNetworkUp;
}
