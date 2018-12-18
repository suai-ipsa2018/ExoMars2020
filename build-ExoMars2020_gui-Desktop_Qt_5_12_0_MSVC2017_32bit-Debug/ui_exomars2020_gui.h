/********************************************************************************
** Form generated from reading UI file 'exomars2020_gui.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXOMARS2020_GUI_H
#define UI_EXOMARS2020_GUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ExoMars2020_gui
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tab_node_carac;
    QWidget *tab_pcarac;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *ExoMars2020_gui)
    {
        if (ExoMars2020_gui->objectName().isEmpty())
            ExoMars2020_gui->setObjectName(QString::fromUtf8("ExoMars2020_gui"));
        ExoMars2020_gui->resize(456, 327);
        centralwidget = new QWidget(ExoMars2020_gui);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab_node_carac = new QWidget();
        tab_node_carac->setObjectName(QString::fromUtf8("tab_node_carac"));
        tabWidget->addTab(tab_node_carac, QString());
        tab_pcarac = new QWidget();
        tab_pcarac->setObjectName(QString::fromUtf8("tab_pcarac"));
        tabWidget->addTab(tab_pcarac, QString());

        verticalLayout->addWidget(tabWidget);

        ExoMars2020_gui->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ExoMars2020_gui);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 456, 26));
        ExoMars2020_gui->setMenuBar(menubar);
        statusbar = new QStatusBar(ExoMars2020_gui);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        ExoMars2020_gui->setStatusBar(statusbar);

        retranslateUi(ExoMars2020_gui);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(ExoMars2020_gui);
    } // setupUi

    void retranslateUi(QMainWindow *ExoMars2020_gui)
    {
        ExoMars2020_gui->setWindowTitle(QApplication::translate("ExoMars2020_gui", "ExoMars2020", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_node_carac), QApplication::translate("ExoMars2020_gui", "Node's caracteristics", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_pcarac), QApplication::translate("ExoMars2020_gui", "Packets' caracteristics", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ExoMars2020_gui: public Ui_ExoMars2020_gui {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXOMARS2020_GUI_H
