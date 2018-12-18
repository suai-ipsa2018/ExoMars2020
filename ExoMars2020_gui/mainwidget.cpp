#include "mainwidget.h"
#include "ui_mainwidget.h"

mainWidget::mainWidget(QWidget *parent) : QWidget(parent), ui(new Ui::mainWidget)
{
    ui->setupUi(this);
}

mainWidget::~mainWidget()
{
    delete ui;
}

void mainWidget::on_add_clicked()
{
    ui->listWidget->addItem("New Node");
}

void mainWidget::on_delete_2_clicked()
{
    delete ui->listWidget->currentItem();
}

void mainWidget::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    ui->listWidget->editItem(item);
}

void mainWidget::on_setNodecarac_clicked()
{
    map_caract[ui->listWidget->currentItem()] = {ui->log_address_sb->value(),
                                                 ui->fsize_sb->value(),
                                                 ui->speed_bytes_line->text(),
                                                 ui->speed_channel_line->text(),
                                                 ui->error_period_line->text()};
}

void mainWidget::on_listWidget_itemClicked(QListWidgetItem *item)
{
    ui->log_address_sb->setValue(map_caract[item].log_add);
    ui->fsize_sb->setValue(map_caract[item].fsize);
    ui->speed_bytes_line->setText(map_caract[item].speed_bytes);
    ui->speed_channel_line->setText(map_caract[item].speed_channel);
    ui->error_period_line->setText(map_caract[item].err_period);
}
