#ifndef SECONDTAB_H
#define SECONDTAB_H

#include <QWidget>

namespace Ui {
class secondTab;
}

class secondTab : public QWidget
{
    Q_OBJECT

public:
    explicit secondTab(QWidget *parent = nullptr);
    ~secondTab();

private:
    Ui::secondTab *ui;
};

#endif // SECONDTAB_H
