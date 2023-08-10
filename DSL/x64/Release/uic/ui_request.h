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
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Request
{
public:

    void setupUi(QWidget *Request)
    {
        if (Request->objectName().isEmpty())
            Request->setObjectName(QString::fromUtf8("Request"));
        Request->resize(400, 300);

        retranslateUi(Request);

        QMetaObject::connectSlotsByName(Request);
    } // setupUi

    void retranslateUi(QWidget *Request)
    {
        Request->setWindowTitle(QApplication::translate("Request", "Request", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Request: public Ui_Request {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REQUEST_H
