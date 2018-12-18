#ifndef SIMUCONTROL_H
#define SIMUCONTROL_H

#include <QWidget>
#include <Windows.h>

namespace Ui {
class SimuControl;
}

class SimuControl : public QWidget
{
    Q_OBJECT

public:
    explicit SimuControl(QWidget *parent = nullptr);
    ~SimuControl();

private slots:
    void on_compileButton_clicked();

    void on_startButton_clicked();

private:
    Ui::SimuControl *ui;
};

#endif // SIMUCONTROL_H
