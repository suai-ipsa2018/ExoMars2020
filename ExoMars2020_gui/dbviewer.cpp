#include "dbviewer.h"
#include "ui_dbviewer.h"

dbViewer::dbViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dbViewer)
{
    ui->setupUi(this);
}

dbViewer::~dbViewer()
{
    delete ui;
}
