#ifndef EXOMARS2020_GUI_H
#define EXOMARS2020_GUI_H

#include <QMainWindow>
#include <QWidget>
#include <iostream>
#include <QTabWidget>

#include "simucontrol.h"
#include "mainwidget.h"
#include "secondtab.h"
#include "dbviewer.h"

namespace Ui {
class ExoMars2020_gui;
}

class ExoMars2020_gui : public QMainWindow
{
    Q_OBJECT

public:
    explicit ExoMars2020_gui(QWidget *parent = nullptr);
    ~ExoMars2020_gui();

private:
    Ui::ExoMars2020_gui *ui;
};

#endif // EXOMARS2020_GUI_H
