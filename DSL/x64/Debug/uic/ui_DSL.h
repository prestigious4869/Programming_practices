/********************************************************************************
** Form generated from reading UI file 'DSL.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DSL_H
#define UI_DSL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DSLClass
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QTextEdit *textEdit_2;
    QTextEdit *textEdit;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;

    void setupUi(QMainWindow *DSLClass)
    {
        if (DSLClass->objectName().isEmpty())
            DSLClass->setObjectName(QString::fromUtf8("DSLClass"));
        DSLClass->resize(1100, 688);
        centralWidget = new QWidget(DSLClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout_2 = new QGridLayout(centralWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        textEdit_2 = new QTextEdit(centralWidget);
        textEdit_2->setObjectName(QString::fromUtf8("textEdit_2"));
        textEdit_2->setFocusPolicy(Qt::StrongFocus);
        textEdit_2->setContextMenuPolicy(Qt::PreventContextMenu);
        textEdit_2->setLineWidth(1);

        gridLayout->addWidget(textEdit_2, 1, 0, 1, 2);

        textEdit = new QTextEdit(centralWidget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setCursorWidth(1);

        gridLayout->addWidget(textEdit, 0, 0, 1, 2);

        horizontalSpacer = new QSpacerItem(60, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 2, 0, 1, 1);

        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setCheckable(false);
        pushButton->setChecked(false);
        pushButton->setAutoRepeat(false);
        pushButton->setAutoExclusive(false);
        pushButton->setAutoDefault(false);

        gridLayout->addWidget(pushButton, 2, 1, 1, 1);

        gridLayout->setRowStretch(0, 2);
        gridLayout->setRowStretch(1, 1);

        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        DSLClass->setCentralWidget(centralWidget);

        retranslateUi(DSLClass);

        pushButton->setDefault(false);


        QMetaObject::connectSlotsByName(DSLClass);
    } // setupUi

    void retranslateUi(QMainWindow *DSLClass)
    {
        DSLClass->setWindowTitle(QApplication::translate("DSLClass", "DSL", nullptr));
        pushButton->setText(QApplication::translate("DSLClass", "\345\217\221\351\200\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DSLClass: public Ui_DSLClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DSL_H
