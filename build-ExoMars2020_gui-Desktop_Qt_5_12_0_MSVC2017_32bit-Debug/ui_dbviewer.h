/********************************************************************************
** Form generated from reading UI file 'dbviewer.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DBVIEWER_H
#define UI_DBVIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_dbViewer
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QComboBox *tablesComboBox;
    QSpacerItem *horizontalSpacer;
    QTableWidget *dbTable;

    void setupUi(QWidget *dbViewer)
    {
        if (dbViewer->objectName().isEmpty())
            dbViewer->setObjectName(QString::fromUtf8("dbViewer"));
        dbViewer->resize(931, 600);
        verticalLayout = new QVBoxLayout(dbViewer);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        tablesComboBox = new QComboBox(dbViewer);
        tablesComboBox->setObjectName(QString::fromUtf8("tablesComboBox"));

        horizontalLayout_3->addWidget(tablesComboBox);

        horizontalSpacer = new QSpacerItem(500, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout_3);

        dbTable = new QTableWidget(dbViewer);
        dbTable->setObjectName(QString::fromUtf8("dbTable"));
        dbTable->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

        verticalLayout->addWidget(dbTable);


        retranslateUi(dbViewer);

        QMetaObject::connectSlotsByName(dbViewer);
    } // setupUi

    void retranslateUi(QWidget *dbViewer)
    {
        dbViewer->setWindowTitle(QApplication::translate("dbViewer", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class dbViewer: public Ui_dbViewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DBVIEWER_H
