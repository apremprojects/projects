/********************************************************************************
** Form generated from reading UI file 'main_dlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAIN_DLG_H
#define UI_MAIN_DLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "path_view_widget.h"

QT_BEGIN_NAMESPACE

class Ui_MainDlgUi
{
public:
    QVBoxLayout *verticalLayout;
    PathViewWidget *path_view;
    QWidget *widget;
    QWidget *widget1;
    QHBoxLayout *horizontalLayout;
    QLabel *x_label;
    QLineEdit *x_box;
    QLabel *y_label;
    QLineEdit *y_box;
    QLabel *theta_label;
    QLineEdit *theta_box;
    QPushButton *reset_button;
    QLabel *label;
    QLineEdit *host_ip;
    QPushButton *connect_btn;

    void setupUi(QDialog *MainDlgUi)
    {
        if (MainDlgUi->objectName().isEmpty())
            MainDlgUi->setObjectName(QString::fromUtf8("MainDlgUi"));
        MainDlgUi->resize(993, 925);
        verticalLayout = new QVBoxLayout(MainDlgUi);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(-1, 0, -1, 0);
        path_view = new PathViewWidget(MainDlgUi);
        path_view->setObjectName(QString::fromUtf8("path_view"));
        path_view->setMinimumSize(QSize(0, 0));
        path_view->setMaximumSize(QSize(16777215, 16777215));

        verticalLayout->addWidget(path_view);

        widget = new QWidget(MainDlgUi);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setMinimumSize(QSize(0, 35));
        widget->setMaximumSize(QSize(16777215, 35));
        widget1 = new QWidget(widget);
        widget1->setObjectName(QString::fromUtf8("widget1"));
        widget1->setGeometry(QRect(152, 2, 826, 32));
        horizontalLayout = new QHBoxLayout(widget1);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        x_label = new QLabel(widget1);
        x_label->setObjectName(QString::fromUtf8("x_label"));
        x_label->setMinimumSize(QSize(0, 30));
        x_label->setMaximumSize(QSize(16777215, 30));

        horizontalLayout->addWidget(x_label);

        x_box = new QLineEdit(widget1);
        x_box->setObjectName(QString::fromUtf8("x_box"));
        x_box->setMinimumSize(QSize(100, 30));
        x_box->setMaximumSize(QSize(100, 30));
        x_box->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(x_box);

        y_label = new QLabel(widget1);
        y_label->setObjectName(QString::fromUtf8("y_label"));
        y_label->setMinimumSize(QSize(0, 30));
        y_label->setMaximumSize(QSize(16777215, 30));

        horizontalLayout->addWidget(y_label);

        y_box = new QLineEdit(widget1);
        y_box->setObjectName(QString::fromUtf8("y_box"));
        y_box->setMinimumSize(QSize(100, 30));
        y_box->setMaximumSize(QSize(100, 30));
        y_box->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(y_box);

        theta_label = new QLabel(widget1);
        theta_label->setObjectName(QString::fromUtf8("theta_label"));
        theta_label->setMinimumSize(QSize(0, 30));
        theta_label->setMaximumSize(QSize(16777215, 30));

        horizontalLayout->addWidget(theta_label);

        theta_box = new QLineEdit(widget1);
        theta_box->setObjectName(QString::fromUtf8("theta_box"));
        theta_box->setMinimumSize(QSize(100, 30));
        theta_box->setMaximumSize(QSize(100, 30));
        theta_box->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(theta_box);

        reset_button = new QPushButton(widget1);
        reset_button->setObjectName(QString::fromUtf8("reset_button"));

        horizontalLayout->addWidget(reset_button);

        label = new QLabel(widget1);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(0, 30));
        label->setMaximumSize(QSize(16777215, 30));

        horizontalLayout->addWidget(label);

        host_ip = new QLineEdit(widget1);
        host_ip->setObjectName(QString::fromUtf8("host_ip"));
        host_ip->setMinimumSize(QSize(100, 30));
        host_ip->setMaximumSize(QSize(100, 30));
        host_ip->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(host_ip);

        connect_btn = new QPushButton(widget1);
        connect_btn->setObjectName(QString::fromUtf8("connect_btn"));
        connect_btn->setMinimumSize(QSize(120, 30));
        connect_btn->setMaximumSize(QSize(120, 30));

        horizontalLayout->addWidget(connect_btn);


        verticalLayout->addWidget(widget);


        retranslateUi(MainDlgUi);

        QMetaObject::connectSlotsByName(MainDlgUi);
    } // setupUi

    void retranslateUi(QDialog *MainDlgUi)
    {
        MainDlgUi->setWindowTitle(QCoreApplication::translate("MainDlgUi", "Romi path viewer", nullptr));
        x_label->setText(QCoreApplication::translate("MainDlgUi", "X", nullptr));
        x_box->setText(QCoreApplication::translate("MainDlgUi", "0", nullptr));
        y_label->setText(QCoreApplication::translate("MainDlgUi", "Y", nullptr));
        y_box->setText(QCoreApplication::translate("MainDlgUi", "0", nullptr));
        theta_label->setText(QCoreApplication::translate("MainDlgUi", "\316\270", nullptr));
        theta_box->setText(QCoreApplication::translate("MainDlgUi", "0", nullptr));
        reset_button->setText(QCoreApplication::translate("MainDlgUi", "Reset", nullptr));
        label->setText(QCoreApplication::translate("MainDlgUi", "Robot (Host address):", nullptr));
        host_ip->setText(QCoreApplication::translate("MainDlgUi", "localhost", nullptr));
        connect_btn->setText(QCoreApplication::translate("MainDlgUi", "connect", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainDlgUi: public Ui_MainDlgUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAIN_DLG_H
