#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <iostream>
#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QLineEdit>
#include <QVector>
#include <QDebug>
#include <QString>

struct caract{
    int log_add;
    int fsize;
    QString speed_bytes;
    QString speed_channel;
    QString err_period;
};

namespace Ui {
class mainWidget;
}

class mainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit mainWidget(QWidget *parent = nullptr);
    ~mainWidget();

private slots:
    void on_add_clicked();
    void on_delete_2_clicked();
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_setNodecarac_clicked();
    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::mainWidget *ui;
    QHash<QListWidgetItem*, caract> map_caract;
};

#endif // MAINWIDGET_H
