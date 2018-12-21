#include "simucontrol.h"
#include "ui_simucontrol.h"
#include <iostream>
#include <strsafe.h>


SimuControl::SimuControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SimuControl)
{
    ui->setupUi(this);
}

SimuControl::~SimuControl()
{
    delete ui;
}

void SimuControl::on_compileButton_clicked()
{
    QString cbt =  ui->partComboBox->currentText();
#ifdef _DEBUG
        system(R"cmd("C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin\MSBuild" ..\Network\Network.vcxproj /t:Build /p:DEFINES=NETWORK_PART=1;IntDir=custom_build_int\;OutDir=custom_build\;TargetName=mast_part;Configuration=Debug;Platform=x86)cmd");
#else
    if (cbt == "All, sequential")
        system(R"cmd("C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin\MSBuild" ..\ExoMars2020\Network\Network.vcxproj /t:Build /p:IntDir=custom_build_int\;OutDir=custom_build\;TargetName=Network;Configuration=Release;Platform=x86;DEFINES=NETWORK_PART=0)cmd");
    else if (cbt == "All, parallel")
    {
        // iterate through
    }
#endif
}

void SimuControl::on_startButton_clicked()
{
    int n =  ui->partComboBox->currentIndex();
    QString cmd(("../ExoMars2020/Network/custom_build/Network_" + std::to_string(n) + ".exe -d -t=\""
                 + std::to_string(ui->timeSpinBox->value()) + ' ' + ui->unitComboBox->currentText().toStdString() + "\"").c_str());

    network_simulation = new QProcess();
    network_simulation->setWorkingDirectory("../ExoMars2020/Network/");

    connect(network_simulation, SIGNAL(readyReadStandardOutput()), this, SLOT(readyReadStandardOutput()));
    connect(network_simulation, SIGNAL(readyReadStandardError()), this, SLOT(readyReadStandardError()));

    network_simulation->start(cmd);
}

void SimuControl::readyReadStandardOutput()
{
    qDebug(network_simulation->readAllStandardOutput());
}

void SimuControl::readyReadStandardError()
{
    qDebug(network_simulation->readAllStandardOutput());
}
