/********************************************************************************
** Form generated from reading UI file 'secondtab.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SECONDTAB_H
#define UI_SECONDTAB_H

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

class Ui_secondTab
{
public:
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QListWidget *listWidget2;
    QVBoxLayout *verticalLayout;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_name_sender;
    QLineEdit *sender_name_line;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_name_recv;
    QLineEdit *recv_name_line;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_psize;
    QSpinBox *psize_sb;
    QVBoxLayout *verticalLayout_6;
    QLabel *label_delay_btw_pckts;
    QLineEdit *delay_btw_pckts_line;
    QVBoxLayout *verticalLayout_7;
    QLabel *label_time_start;
    QLineEdit *time_start_line;
    QLabel *label_nb_pckts;
    QSpinBox *nb_pckts_sb;
    QPushButton *setPcktcarac;
    QPushButton *add2;
    QPushButton *delete2;

    void setupUi(QWidget *secondTab)
    {
        if (secondTab->objectName().isEmpty())
            secondTab->setObjectName(QString::fromUtf8("secondTab"));
        secondTab->resize(497, 324);
        verticalLayout_2 = new QVBoxLayout(secondTab);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        listWidget2 = new QListWidget(secondTab);
        listWidget2->setObjectName(QString::fromUtf8("listWidget2"));

        horizontalLayout_2->addWidget(listWidget2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label_name_sender = new QLabel(secondTab);
        label_name_sender->setObjectName(QString::fromUtf8("label_name_sender"));

        verticalLayout_3->addWidget(label_name_sender);

        sender_name_line = new QLineEdit(secondTab);
        sender_name_line->setObjectName(QString::fromUtf8("sender_name_line"));

        verticalLayout_3->addWidget(sender_name_line);


        verticalLayout->addLayout(verticalLayout_3);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        label_name_recv = new QLabel(secondTab);
        label_name_recv->setObjectName(QString::fromUtf8("label_name_recv"));

        verticalLayout_4->addWidget(label_name_recv);

        recv_name_line = new QLineEdit(secondTab);
        recv_name_line->setObjectName(QString::fromUtf8("recv_name_line"));

        verticalLayout_4->addWidget(recv_name_line);


        verticalLayout->addLayout(verticalLayout_4);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        label_psize = new QLabel(secondTab);
        label_psize->setObjectName(QString::fromUtf8("label_psize"));

        verticalLayout_5->addWidget(label_psize);

        psize_sb = new QSpinBox(secondTab);
        psize_sb->setObjectName(QString::fromUtf8("psize_sb"));
        psize_sb->setMinimum(1);
        psize_sb->setMaximum(1000000);

        verticalLayout_5->addWidget(psize_sb);


        verticalLayout->addLayout(verticalLayout_5);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        label_delay_btw_pckts = new QLabel(secondTab);
        label_delay_btw_pckts->setObjectName(QString::fromUtf8("label_delay_btw_pckts"));

        verticalLayout_6->addWidget(label_delay_btw_pckts);

        delay_btw_pckts_line = new QLineEdit(secondTab);
        delay_btw_pckts_line->setObjectName(QString::fromUtf8("delay_btw_pckts_line"));

        verticalLayout_6->addWidget(delay_btw_pckts_line);


        verticalLayout->addLayout(verticalLayout_6);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        label_time_start = new QLabel(secondTab);
        label_time_start->setObjectName(QString::fromUtf8("label_time_start"));

        verticalLayout_7->addWidget(label_time_start);

        time_start_line = new QLineEdit(secondTab);
        time_start_line->setObjectName(QString::fromUtf8("time_start_line"));

        verticalLayout_7->addWidget(time_start_line);


        verticalLayout->addLayout(verticalLayout_7);

        label_nb_pckts = new QLabel(secondTab);
        label_nb_pckts->setObjectName(QString::fromUtf8("label_nb_pckts"));

        verticalLayout->addWidget(label_nb_pckts);

        nb_pckts_sb = new QSpinBox(secondTab);
        nb_pckts_sb->setObjectName(QString::fromUtf8("nb_pckts_sb"));
        nb_pckts_sb->setMinimum(1);
        nb_pckts_sb->setMaximum(1000);

        verticalLayout->addWidget(nb_pckts_sb);

        setPcktcarac = new QPushButton(secondTab);
        setPcktcarac->setObjectName(QString::fromUtf8("setPcktcarac"));

        verticalLayout->addWidget(setPcktcarac);


        horizontalLayout_2->addLayout(verticalLayout);


        verticalLayout_2->addLayout(horizontalLayout_2);

        add2 = new QPushButton(secondTab);
        add2->setObjectName(QString::fromUtf8("add2"));

        verticalLayout_2->addWidget(add2);

        delete2 = new QPushButton(secondTab);
        delete2->setObjectName(QString::fromUtf8("delete2"));

        verticalLayout_2->addWidget(delete2);


        retranslateUi(secondTab);

        QMetaObject::connectSlotsByName(secondTab);
    } // setupUi

    void retranslateUi(QWidget *secondTab)
    {
        secondTab->setWindowTitle(QApplication::translate("secondTab", "Form", nullptr));
        label_name_sender->setText(QApplication::translate("secondTab", "Sender's name ", nullptr));
        sender_name_line->setPlaceholderText(QApplication::translate("secondTab", "Write the name of the node who sends the packet", nullptr));
        label_name_recv->setText(QApplication::translate("secondTab", "Receiver's name :", nullptr));
        recv_name_line->setPlaceholderText(QApplication::translate("secondTab", "Write the name of the node who receives the packet", nullptr));
        label_psize->setText(QApplication::translate("secondTab", "Packet's size (value greater or equal than 1) :", nullptr));
        label_delay_btw_pckts->setText(QApplication::translate("secondTab", "Delay after sending the following packets (units in abbreviated form) :", nullptr));
        delay_btw_pckts_line->setPlaceholderText(QApplication::translate("secondTab", "Time with units", nullptr));
        label_time_start->setText(QApplication::translate("secondTab", "Time start (units in abbreviated form) :", nullptr));
        time_start_line->setPlaceholderText(QApplication::translate("secondTab", "Time with units", nullptr));
        label_nb_pckts->setText(QApplication::translate("secondTab", "Number of packets to send :", nullptr));
        setPcktcarac->setText(QApplication::translate("secondTab", "Set packet's caracteristics", nullptr));
        add2->setText(QApplication::translate("secondTab", "Add Packet", nullptr));
        delete2->setText(QApplication::translate("secondTab", "Delete Packet", nullptr));
    } // retranslateUi

};

namespace Ui {
    class secondTab: public Ui_secondTab {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SECONDTAB_H
