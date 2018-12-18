/********************************************************************************
** Form generated from reading UI file 'mainwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWIDGET_H
#define UI_MAINWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_mainWidget
{
public:
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QListWidget *listWidget;
    QVBoxLayout *verticalLayout;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_log_address;
    QSpinBox *log_address_sb;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_fsize;
    QSpinBox *fsize_sb;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_speed_bytes;
    QLineEdit *speed_bytes_line;
    QVBoxLayout *verticalLayout_6;
    QLabel *label_speed_channel;
    QLineEdit *speed_channel_line;
    QVBoxLayout *verticalLayout_7;
    QLabel *label_error_period;
    QLineEdit *error_period_line;
    QPushButton *setNodecarac;
    QPushButton *add;
    QPushButton *delete_2;

    void setupUi(QWidget *mainWidget)
    {
        if (mainWidget->objectName().isEmpty())
            mainWidget->setObjectName(QString::fromUtf8("mainWidget"));
        mainWidget->resize(501, 332);
        verticalLayout_2 = new QVBoxLayout(mainWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        listWidget = new QListWidget(mainWidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        horizontalLayout_2->addWidget(listWidget);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label_log_address = new QLabel(mainWidget);
        label_log_address->setObjectName(QString::fromUtf8("label_log_address"));

        verticalLayout_3->addWidget(label_log_address);

        log_address_sb = new QSpinBox(mainWidget);
        log_address_sb->setObjectName(QString::fromUtf8("log_address_sb"));
        log_address_sb->setMinimum(32);
        log_address_sb->setMaximum(254);

        verticalLayout_3->addWidget(log_address_sb);


        verticalLayout->addLayout(verticalLayout_3);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        label_fsize = new QLabel(mainWidget);
        label_fsize->setObjectName(QString::fromUtf8("label_fsize"));

        verticalLayout_4->addWidget(label_fsize);

        fsize_sb = new QSpinBox(mainWidget);
        fsize_sb->setObjectName(QString::fromUtf8("fsize_sb"));
        fsize_sb->setMinimum(1);
        fsize_sb->setMaximum(16);

        verticalLayout_4->addWidget(fsize_sb);


        verticalLayout->addLayout(verticalLayout_4);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        label_speed_bytes = new QLabel(mainWidget);
        label_speed_bytes->setObjectName(QString::fromUtf8("label_speed_bytes"));

        verticalLayout_5->addWidget(label_speed_bytes);

        speed_bytes_line = new QLineEdit(mainWidget);
        speed_bytes_line->setObjectName(QString::fromUtf8("speed_bytes_line"));

        verticalLayout_5->addWidget(speed_bytes_line);


        verticalLayout->addLayout(verticalLayout_5);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        label_speed_channel = new QLabel(mainWidget);
        label_speed_channel->setObjectName(QString::fromUtf8("label_speed_channel"));

        verticalLayout_6->addWidget(label_speed_channel);

        speed_channel_line = new QLineEdit(mainWidget);
        speed_channel_line->setObjectName(QString::fromUtf8("speed_channel_line"));

        verticalLayout_6->addWidget(speed_channel_line);


        verticalLayout->addLayout(verticalLayout_6);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        label_error_period = new QLabel(mainWidget);
        label_error_period->setObjectName(QString::fromUtf8("label_error_period"));

        verticalLayout_7->addWidget(label_error_period);

        error_period_line = new QLineEdit(mainWidget);
        error_period_line->setObjectName(QString::fromUtf8("error_period_line"));

        verticalLayout_7->addWidget(error_period_line);


        verticalLayout->addLayout(verticalLayout_7);

        setNodecarac = new QPushButton(mainWidget);
        setNodecarac->setObjectName(QString::fromUtf8("setNodecarac"));

        verticalLayout->addWidget(setNodecarac);


        horizontalLayout_2->addLayout(verticalLayout);


        verticalLayout_2->addLayout(horizontalLayout_2);

        add = new QPushButton(mainWidget);
        add->setObjectName(QString::fromUtf8("add"));

        verticalLayout_2->addWidget(add);

        delete_2 = new QPushButton(mainWidget);
        delete_2->setObjectName(QString::fromUtf8("delete_2"));

        verticalLayout_2->addWidget(delete_2);


        retranslateUi(mainWidget);

        QMetaObject::connectSlotsByName(mainWidget);
    } // setupUi

    void retranslateUi(QWidget *mainWidget)
    {
        mainWidget->setWindowTitle(QApplication::translate("mainWidget", "ExoMars2020", nullptr));
        label_log_address->setText(QApplication::translate("mainWidget", "Logical address (value from 32 to 254) :", nullptr));
        label_fsize->setText(QApplication::translate("mainWidget", "Size of frames in bit (value from 1 to 16) :", nullptr));
        label_speed_bytes->setText(QApplication::translate("mainWidget", "Speed of sending bytes (units in abbreviated form) :", nullptr));
        speed_bytes_line->setPlaceholderText(QApplication::translate("mainWidget", "Time with units", nullptr));
        label_speed_channel->setText(QApplication::translate("mainWidget", "Speed of the channel (units in abbreviated form) :", nullptr));
        speed_channel_line->setPlaceholderText(QApplication::translate("mainWidget", "Time with units", nullptr));
        label_error_period->setText(QApplication::translate("mainWidget", "Error period (units in abbreviated form) :", nullptr));
        error_period_line->setPlaceholderText(QApplication::translate("mainWidget", "Time with units", nullptr));
        setNodecarac->setText(QApplication::translate("mainWidget", "Set caracteristics", nullptr));
        add->setText(QApplication::translate("mainWidget", "Add Node", nullptr));
        delete_2->setText(QApplication::translate("mainWidget", "Delete Node", nullptr));
    } // retranslateUi

};

namespace Ui {
    class mainWidget: public Ui_mainWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWIDGET_H
