#ifndef DBVIEWER_H
#define DBVIEWER_H

#include <QWidget>
#include <iostream>
#include <sqlite3.h>

namespace Ui {
class dbViewer;
}

class dbViewer : public QWidget
{
    Q_OBJECT

public:
    explicit dbViewer(QWidget *parent = nullptr);
    ~dbViewer();

private slots:
    void on_tablesComboBox_currentIndexChanged(const QString &table);

private:
    Ui::dbViewer *ui;
    sqlite3* db;
};

#endif // DBVIEWER_H
