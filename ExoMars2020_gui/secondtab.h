#ifndef SECONDTAB_H
#define SECONDTAB_H

#include <iostream>
#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>

struct packet_caract{
    QString sender_name;
    QString recv_name;
    int psize;
    QString delay_btw_bytes;
    QString time_start;
    int nb_packets;
};

namespace Ui {
class secondTab;
}

class secondTab : public QWidget
{
    Q_OBJECT

public:
    explicit secondTab(QWidget *parent = nullptr);
    ~secondTab();

private slots:
    void on_add2_clicked();
    void on_delete2_clicked();
    void on_setPcktcarac_clicked();

    void on_listWidget2_itemClicked(QListWidgetItem *item);

private:
    Ui::secondTab *ui;
    QHash<QListWidgetItem*, packet_caract> map_packet_caract;
};

#endif // SECONDTAB_H
