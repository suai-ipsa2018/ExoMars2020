#include "simucontrol.h"
#include "ui_simucontrol.h"

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
    int n =  ui->partComboBox->currentIndex();
#ifdef _DEBUG
        system(R"cmd("C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin\MSBuild" ..\Network\Network.vcxproj /t:Build /p:DEFINES=NETWORK_PART=1;IntDir=custom_build_int\;OutDir=custom_build\;TargetName=mast_part;Configuration=Debug;Platform=x86)cmd");
#else
        system((R"cmd("C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin\MSBuild" ..\ExoMars2020\Network\Network.vcxproj /t:Build /p:IntDir=custom_build_int\;OutDir=custom_build\;TargetName=Network_)cmd" + std::to_string(n) + R"cmd(;Configuration=Release;Platform=x86;DEFINES=NETWORK_PART=)cmd" + std::to_string(n)).c_str());
#endif
}

void SimuControl::on_startButton_clicked()
{
    int n =  ui->partComboBox->currentIndex();
    LPSTARTUPINFOA si;
    PROCESS_INFORMATION pi;

    std::string path("../ExoMars2020/Network/custom_build/Network_" + std::to_string(n));
    std::string cla("-d -t=\"300 us\"");

    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));

    CreateProcessA(path.c_str(),
        const_cast<char*>((path + ' ' + cla).c_str()), nullptr, nullptr, FALSE, 0, nullptr, "../Network", si, &pi);
}
