#include "exomars2020_gui.h"
#include "ui_exomars2020_gui.h"

ExoMars2020_gui::ExoMars2020_gui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ExoMars2020_gui)
{
    ui->setupUi(this);
}

ExoMars2020_gui::~ExoMars2020_gui()
{
    delete ui;
}
