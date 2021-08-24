/********************************************************************************
** Form generated from reading UI file 'QtSearchBox.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTSEARCHBOX_H
#define UI_QTSEARCHBOX_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtSearchBox
{
public:
    QVBoxLayout *verticalLayout;
    QLineEdit *lineEdit;
    QListWidget *listWidget;

    void setupUi(QWidget *QtSearchBox)
    {
        if (QtSearchBox->objectName().isEmpty())
            QtSearchBox->setObjectName(QString::fromUtf8("QtSearchBox"));
        QtSearchBox->resize(300, 240);
        verticalLayout = new QVBoxLayout(QtSearchBox);
        verticalLayout->setSpacing(1);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        lineEdit = new QLineEdit(QtSearchBox);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        verticalLayout->addWidget(lineEdit);

        listWidget = new QListWidget(QtSearchBox);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        verticalLayout->addWidget(listWidget);


        retranslateUi(QtSearchBox);

        QMetaObject::connectSlotsByName(QtSearchBox);
    } // setupUi

    void retranslateUi(QWidget *QtSearchBox)
    {
        QtSearchBox->setWindowTitle(QCoreApplication::translate("QtSearchBox", "QtSearchBox", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtSearchBox: public Ui_QtSearchBox {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTSEARCHBOX_H
