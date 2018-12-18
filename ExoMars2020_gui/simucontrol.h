#ifndef SIMUCONTROL_H
#define SIMUCONTROL_H

#include <QWidget>

namespace Ui {
class SimuControl;
}

class SimuControl : public QWidget
{
    Q_OBJECT

public:
    explicit SimuControl(QWidget *parent = nullptr);
    ~SimuControl();

private:
    Ui::SimuControl *ui;
};

#endif // SIMUCONTROL_H
