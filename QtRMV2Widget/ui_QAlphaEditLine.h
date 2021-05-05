/********************************************************************************
** Form generated from reading UI file 'QAlphaEditLine.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QALPHAEDITLINE_H
#define UI_QALPHAEDITLINE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QAlphaEditLine
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QGridLayout *gridLayout_4;
    QLabel *label;
    QLineEdit *lineEditFlagValue;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer;
    QComboBox *comboBoxKnownValues;
    QLabel *label_3;

    void setupUi(QWidget *QAlphaEditLine)
    {
        if (QAlphaEditLine->objectName().isEmpty())
            QAlphaEditLine->setObjectName(QString::fromUtf8("QAlphaEditLine"));
        QAlphaEditLine->resize(831, 252);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QAlphaEditLine->sizePolicy().hasHeightForWidth());
        QAlphaEditLine->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(QAlphaEditLine);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox = new QGroupBox(QAlphaEditLine);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        gridLayout_4 = new QGridLayout();
        gridLayout_4->setSpacing(6);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_4->setHorizontalSpacing(7);
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_4->addWidget(label, 1, 1, 1, 1);

        lineEditFlagValue = new QLineEdit(groupBox);
        lineEditFlagValue->setObjectName(QString::fromUtf8("lineEditFlagValue"));
        lineEditFlagValue->setMinimumSize(QSize(100, 0));
        lineEditFlagValue->setMaximumSize(QSize(200, 16777215));
        lineEditFlagValue->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_4->addWidget(lineEditFlagValue, 1, 5, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy1);
        label_2->setLayoutDirection(Qt::LeftToRight);
        label_2->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(label_2, 0, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer, 1, 0, 1, 1);

        comboBoxKnownValues = new QComboBox(groupBox);
        comboBoxKnownValues->setObjectName(QString::fromUtf8("comboBoxKnownValues"));
        comboBoxKnownValues->setMinimumSize(QSize(150, 0));
        comboBoxKnownValues->setMaximumSize(QSize(200, 16777215));

        gridLayout_4->addWidget(comboBoxKnownValues, 1, 2, 1, 1);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_4->addWidget(label_3, 0, 5, 1, 1);


        horizontalLayout->addLayout(gridLayout_4);


        verticalLayout->addWidget(groupBox);


        retranslateUi(QAlphaEditLine);

        QMetaObject::connectSlotsByName(QAlphaEditLine);
    } // setupUi

    void retranslateUi(QWidget *QAlphaEditLine)
    {
        QAlphaEditLine->setWindowTitle(QCoreApplication::translate("QAlphaEditLine", "QTextureEditLine", nullptr));
        groupBox->setTitle(QCoreApplication::translate("QAlphaEditLine", "Flags", nullptr));
        label->setText(QCoreApplication::translate("QAlphaEditLine", "Alpha Flags:", nullptr));
        label_2->setText(QCoreApplication::translate("QAlphaEditLine", "Known values:", nullptr));
        label_3->setText(QCoreApplication::translate("QAlphaEditLine", "Flag Value:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QAlphaEditLine: public Ui_QAlphaEditLine {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QALPHAEDITLINE_H
