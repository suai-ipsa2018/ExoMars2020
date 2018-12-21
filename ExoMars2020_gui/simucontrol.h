#ifndef SIMUCONTROL_H
#define SIMUCONTROL_H

#include <QWidget>
#include <QProcess>
#include <QDebug>

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

    void readyReadStandardOutput();
    void readyReadStandardError();

private:
    Ui::SimuControl *ui;
    QProcess* network_simulation;
};

#endif // SIMUCONTROL_H
