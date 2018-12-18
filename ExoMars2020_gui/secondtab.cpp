#include "secondtab.h"
#include "ui_secondtab.h"

secondTab::secondTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::secondTab)
{
    ui->setupUi(this);
}

secondTab::~secondTab()
{
    delete ui;
}
