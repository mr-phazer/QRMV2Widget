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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtSearchBox
{
public:
    QVBoxLayout *verticalLayout;
    QLineEdit *lineEdit;
    QListWidget *listWidget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QWidget *QtSearchBox)
    {
        if (QtSearchBox->objectName().isEmpty())
            QtSearchBox->setObjectName(QString::fromUtf8("QtSearchBox"));
        QtSearchBox->resize(300, 240);
        verticalLayout = new QVBoxLayout(QtSearchBox);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(1, 1, 1, -1);
        lineEdit = new QLineEdit(QtSearchBox);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        verticalLayout->addWidget(lineEdit);

        listWidget = new QListWidget(QtSearchBox);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        verticalLayout->addWidget(listWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton = new QPushButton(QtSearchBox);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setMaximumSize(QSize(60, 20));

        horizontalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(QtSearchBox);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setMaximumSize(QSize(60, 20));

        horizontalLayout->addWidget(pushButton_2);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(QtSearchBox);

        QMetaObject::connectSlotsByName(QtSearchBox);
    } // setupUi

    void retranslateUi(QWidget *QtSearchBox)
    {
        QtSearchBox->setWindowTitle(QCoreApplication::translate("QtSearchBox", "QtSearchBox", nullptr));
        pushButton->setText(QCoreApplication::translate("QtSearchBox", "Close", nullptr));
        pushButton_2->setText(QCoreApplication::translate("QtSearchBox", "Select", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtSearchBox: public Ui_QtSearchBox {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTSEARCHBOX_H
