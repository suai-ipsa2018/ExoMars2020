/********************************************************************************
** Form generated from reading UI file 'simucontrol.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIMUCONTROL_H
#define UI_SIMUCONTROL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SimuControl
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_2;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QComboBox *partComboBox;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QDoubleSpinBox *timeSpinBox;
    QComboBox *unitComboBox;
    QHBoxLayout *horizontalLayout;
    QPushButton *compileButton;
    QPushButton *startButton;
    QSpacerItem *verticalSpacer;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QWidget *SimuControl)
    {
        if (SimuControl->objectName().isEmpty())
            SimuControl->setObjectName(QString::fromUtf8("SimuControl"));
        SimuControl->resize(647, 370);
        gridLayout = new QGridLayout(SimuControl);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 2, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 1, 0, 1, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_2 = new QLabel(SimuControl);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_3->addWidget(label_2);

        partComboBox = new QComboBox(SimuControl);
        partComboBox->addItem(QString());
        partComboBox->setObjectName(QString::fromUtf8("partComboBox"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(partComboBox->sizePolicy().hasHeightForWidth());
        partComboBox->setSizePolicy(sizePolicy);

        horizontalLayout_3->addWidget(partComboBox);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(SimuControl);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(label);

        timeSpinBox = new QDoubleSpinBox(SimuControl);
        timeSpinBox->setObjectName(QString::fromUtf8("timeSpinBox"));
        timeSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        timeSpinBox->setMaximum(99999999999999995164818811802792197885196090803013355167206819763650035712.000000000000000);

        horizontalLayout_2->addWidget(timeSpinBox);

        unitComboBox = new QComboBox(SimuControl);
        unitComboBox->addItem(QString());
        unitComboBox->addItem(QString());
        unitComboBox->addItem(QString());
        unitComboBox->addItem(QString());
        unitComboBox->addItem(QString());
        unitComboBox->addItem(QString());
        unitComboBox->setObjectName(QString::fromUtf8("unitComboBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(unitComboBox->sizePolicy().hasHeightForWidth());
        unitComboBox->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(unitComboBox);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(36);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, -1, 0, -1);
        compileButton = new QPushButton(SimuControl);
        compileButton->setObjectName(QString::fromUtf8("compileButton"));

        horizontalLayout->addWidget(compileButton);

        startButton = new QPushButton(SimuControl);
        startButton->setObjectName(QString::fromUtf8("startButton"));

        horizontalLayout->addWidget(startButton);


        verticalLayout_2->addLayout(horizontalLayout);


        gridLayout->addLayout(verticalLayout_2, 1, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 0, 1, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_2, 2, 1, 1, 1);


        retranslateUi(SimuControl);

        QMetaObject::connectSlotsByName(SimuControl);
    } // setupUi

    void retranslateUi(QWidget *SimuControl)
    {
        SimuControl->setWindowTitle(QApplication::translate("SimuControl", "Form", nullptr));
        label_2->setText(QApplication::translate("SimuControl", "Part to simulate :", nullptr));
        partComboBox->setItemText(0, QApplication::translate("SimuControl", "All", nullptr));

        label->setText(QApplication::translate("SimuControl", "Simlulation duration :", nullptr));
        unitComboBox->setItemText(0, QApplication::translate("SimuControl", "s", nullptr));
        unitComboBox->setItemText(1, QApplication::translate("SimuControl", "ms", nullptr));
        unitComboBox->setItemText(2, QApplication::translate("SimuControl", "us", nullptr));
        unitComboBox->setItemText(3, QApplication::translate("SimuControl", "ns", nullptr));
        unitComboBox->setItemText(4, QApplication::translate("SimuControl", "ps", nullptr));
        unitComboBox->setItemText(5, QApplication::translate("SimuControl", "fs", nullptr));

        compileButton->setText(QApplication::translate("SimuControl", "Compile", nullptr));
        startButton->setText(QApplication::translate("SimuControl", "Start", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SimuControl: public Ui_SimuControl {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIMUCONTROL_H
