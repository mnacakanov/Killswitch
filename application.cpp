#include "application.h"

Application::Application()
{
    WMIapplication.initNetworkAdapterInterfacing();
    numInterfaces = WMIapplication.getConnectedNetworkAdapterObj(networkAdapters);
    quitButton = new QPushButton(tr("&Quit"));
    connect(quitButton, SIGNAL(clicked()), qApp, SLOT(quit()));

    widgetWidth = 250;
    widgetHeight = 80;
    killSwitchCheckBox = new QCheckBox(tr("Kill Switch"));
    killSwitchCheckBox->setChecked(false);
    connect(killSwitchCheckBox, SIGNAL(clicked()), this, SLOT(killSwitchEngaged()));
    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget(quitButton);
    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addStretch();
    topLayout->addWidget(killSwitchCheckBox);
    topLayout->addStretch();
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(bottomLayout);
    setLayout(mainLayout);
    setWindowTitle(tr("Killswitch"));
    this->resize(widgetWidth, widgetHeight);
}

void Application::killSwitchEngaged(void)
{
    if (killSwitchCheckBox->isChecked())
    {
        qDebug() << "Kisswitch engaged";
        for(int i = 0; i < networkAdapters.size(); i++)
        {
            WMIapplication.executeMethod(networkAdapters[i], "Disable");
        }
    }
    else
    {
        qDebug() << "Kisswitch disengaged";
        for(int i = 0; i < networkAdapters.size(); i++)
        {
            WMIapplication.executeMethod(networkAdapters[i], "Enable");
        }
    }
}

bool Application::checkNetworkConditions(void)
{
    bool isNetworkUp = true;
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

Application::~Application()
{
}
