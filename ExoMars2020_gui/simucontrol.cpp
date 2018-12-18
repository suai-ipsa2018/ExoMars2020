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
