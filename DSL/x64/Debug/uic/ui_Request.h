/********************************************************************************
** Form generated from reading UI file 'Request.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REQUEST_H
#define UI_REQUEST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Request
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;

    void setupUi(QMainWindow *Request)
    {
        if (Request->objectName().isEmpty())
            Request->setObjectName(QString::fromUtf8("Request"));
        Request->resize(478, 172);
        centralwidget = new QWidget(Request);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        gridLayout->addWidget(pushButton, 2, 0, 1, 1);

        pushButton_2 = new QPushButton(centralwidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        gridLayout->addWidget(pushButton_2, 2, 1, 1, 1);

        lineEdit = new QLineEdit(centralwidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setFocusPolicy(Qt::StrongFocus);

        gridLayout->addWidget(lineEdit, 1, 0, 1, 2);

        lineEdit_2 = new QLineEdit(centralwidget);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setFrame(false);
        lineEdit_2->setDragEnabled(false);
        lineEdit_2->setReadOnly(true);

        gridLayout->addWidget(lineEdit_2, 0, 0, 1, 2);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        Request->setCentralWidget(centralwidget);

        retranslateUi(Request);

        QMetaObject::connectSlotsByName(Request);
    } // setupUi

    void retranslateUi(QMainWindow *Request)
    {
        Request->setWindowTitle(QApplication::translate("Request", "\346\211\223\345\274\200", nullptr));
        pushButton->setText(QApplication::translate("Request", "\347\241\256\345\256\232", nullptr));
#ifndef QT_NO_SHORTCUT
        pushButton->setShortcut(QApplication::translate("Request", "Return", nullptr));
#endif // QT_NO_SHORTCUT
        pushButton_2->setText(QApplication::translate("Request", "\345\217\226\346\266\210", nullptr));
        lineEdit_2->setText(QApplication::translate("Request", "\350\276\223\345\205\245\346\240\274\345\274\217\357\274\232[auto ]filename[ testfilename]:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Request: public Ui_Request {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REQUEST_H
