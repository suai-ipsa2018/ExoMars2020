#include "exomars2020_gui.h"
#include "ui_exomars2020_gui.h"

ExoMars2020_gui::ExoMars2020_gui(QWidget *parent) : QMainWindow(parent), ui(new Ui::ExoMars2020_gui)
{
    ui->setupUi(this);

    SimuControl* simu_widget = new SimuControl;
    mainWidget* node_widget = new mainWidget;
    secondTab* packet_widget = new secondTab;
    dbViewer* db_widget = new dbViewer;


    ui->tabWidget->removeTab(1);
    ui->tabWidget->removeTab(0);
    ui->tabWidget->insertTab(0, simu_widget, "Simulation Control");
    ui->tabWidget->insertTab(1, node_widget, "Node's caracteristics");
    ui->tabWidget->insertTab(2, packet_widget, "Packet's caracteristics");
    ui->tabWidget->insertTab(3, db_widget, "Database Viewer");
}

ExoMars2020_gui::~ExoMars2020_gui()
{
    delete ui;
}

