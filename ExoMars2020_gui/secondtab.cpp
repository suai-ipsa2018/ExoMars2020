#include "secondtab.h"
#include "ui_secondtab.h"

secondTab::secondTab(QWidget *parent) : QWidget(parent), ui(new Ui::secondTab)
{
    ui->setupUi(this);
}

secondTab::~secondTab()
{
    delete ui;
}

void secondTab::on_add2_clicked()
{
    ui->listWidget2->addItem("From Node to Node");
}

void secondTab::on_delete2_clicked()
{
    delete ui->listWidget2->currentItem();
}

void secondTab::on_setPcktcarac_clicked()
{
    map_packet_caract[ui->listWidget2->currentItem()] = {ui->sender_name_line->text(),
                                                         ui->recv_name_line->text(),
                                                         ui->psize_sb->value(),
                                                         ui->delay_btw_pckts_line->text(),
                                                         ui->time_start_line->text(),
                                                         ui->nb_pckts_sb->value()};
    ui->listWidget2->currentItem()->setText("From " + ui->sender_name_line->text() + " to " + ui->recv_name_line->text());
}

void secondTab::on_listWidget2_itemClicked(QListWidgetItem *item)
{
    ui->sender_name_line->setText(map_packet_caract[item].sender_name);
    ui->recv_name_line->setText(map_packet_caract[item].recv_name);
    ui->psize_sb->setValue(map_packet_caract[item].psize);
    ui->delay_btw_pckts_line->setText(map_packet_caract[item].delay_btw_bytes);
    ui->time_start_line->setText(map_packet_caract[item].time_start);
    ui->nb_pckts_sb->setValue(map_packet_caract[item].nb_packets);
}
