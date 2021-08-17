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
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QAlphaEditLine
{
public:
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QGridLayout *gridLayout;
    QLineEdit *lineEdit_TextureFolder;
    QLabel *label_5;
    QLabel *label;
    QLineEdit *lineEdit_MeshName;
    QLabel *label_3;
    QLabel *label_2;
    QLabel *label_6;
    QLabel *label_4;
    QLineEdit *lineEditFlagValue;
    QComboBox *comboBoxKnownValues;
    QLineEdit *lineEdit_ShaderName;

    void setupUi(QWidget *QAlphaEditLine)
    {
        if (QAlphaEditLine->objectName().isEmpty())
            QAlphaEditLine->setObjectName(QString::fromUtf8("QAlphaEditLine"));
        QAlphaEditLine->resize(1290, 150);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QAlphaEditLine->sizePolicy().hasHeightForWidth());
        QAlphaEditLine->setSizePolicy(sizePolicy);
        horizontalLayout_2 = new QHBoxLayout(QAlphaEditLine);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        groupBox = new QGroupBox(QAlphaEditLine);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy1);
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(4, 4, 4, 4);
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        lineEdit_TextureFolder = new QLineEdit(groupBox);
        lineEdit_TextureFolder->setObjectName(QString::fromUtf8("lineEdit_TextureFolder"));
        lineEdit_TextureFolder->setReadOnly(true);

        gridLayout->addWidget(lineEdit_TextureFolder, 0, 1, 1, 4);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy2);
        label_5->setAlignment(Qt::AlignBottom|Qt::AlignLeading|Qt::AlignLeft);

        gridLayout->addWidget(label_5, 0, 0, 1, 1);

        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        sizePolicy2.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy2);

        gridLayout->addWidget(label, 1, 5, 1, 1);

        lineEdit_MeshName = new QLineEdit(groupBox);
        lineEdit_MeshName->setObjectName(QString::fromUtf8("lineEdit_MeshName"));
        sizePolicy1.setHeightForWidth(lineEdit_MeshName->sizePolicy().hasHeightForWidth());
        lineEdit_MeshName->setSizePolicy(sizePolicy1);
        lineEdit_MeshName->setMinimumSize(QSize(0, 0));
        lineEdit_MeshName->setReadOnly(true);

        gridLayout->addWidget(lineEdit_MeshName, 2, 1, 1, 3);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        sizePolicy2.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy2);

        gridLayout->addWidget(label_3, 2, 5, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy3);
        label_2->setLayoutDirection(Qt::LeftToRight);
        label_2->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_2, 0, 6, 1, 1);

        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setStyleSheet(QString::fromUtf8("border-top-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));"));

        gridLayout->addWidget(label_6, 2, 0, 1, 1);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setAlignment(Qt::AlignBottom|Qt::AlignLeading|Qt::AlignLeft);

        gridLayout->addWidget(label_4, 1, 0, 1, 1);

        lineEditFlagValue = new QLineEdit(groupBox);
        lineEditFlagValue->setObjectName(QString::fromUtf8("lineEditFlagValue"));
        lineEditFlagValue->setMinimumSize(QSize(100, 0));
        lineEditFlagValue->setMaximumSize(QSize(200, 16777215));
        lineEditFlagValue->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(lineEditFlagValue, 2, 6, 1, 1);

        comboBoxKnownValues = new QComboBox(groupBox);
        comboBoxKnownValues->setObjectName(QString::fromUtf8("comboBoxKnownValues"));
        comboBoxKnownValues->setMinimumSize(QSize(150, 0));
        comboBoxKnownValues->setMaximumSize(QSize(200, 16777215));

        gridLayout->addWidget(comboBoxKnownValues, 1, 6, 1, 1);

        lineEdit_ShaderName = new QLineEdit(groupBox);
        lineEdit_ShaderName->setObjectName(QString::fromUtf8("lineEdit_ShaderName"));
        sizePolicy1.setHeightForWidth(lineEdit_ShaderName->sizePolicy().hasHeightForWidth());
        lineEdit_ShaderName->setSizePolicy(sizePolicy1);
        lineEdit_ShaderName->setMinimumSize(QSize(0, 0));
        lineEdit_ShaderName->setReadOnly(true);

        gridLayout->addWidget(lineEdit_ShaderName, 1, 1, 1, 3);


        horizontalLayout->addLayout(gridLayout);


        horizontalLayout_2->addWidget(groupBox);


        retranslateUi(QAlphaEditLine);

        QMetaObject::connectSlotsByName(QAlphaEditLine);
    } // setupUi

    void retranslateUi(QWidget *QAlphaEditLine)
    {
        QAlphaEditLine->setWindowTitle(QCoreApplication::translate("QAlphaEditLine", "QTextureEditLine", nullptr));
        groupBox->setTitle(QCoreApplication::translate("QAlphaEditLine", "Mesh Info", nullptr));
        lineEdit_TextureFolder->setText(QString());
        lineEdit_TextureFolder->setPlaceholderText(QCoreApplication::translate("QAlphaEditLine", "internal texture folder", nullptr));
        label_5->setText(QCoreApplication::translate("QAlphaEditLine", "Texture Folder", nullptr));
        label->setText(QCoreApplication::translate("QAlphaEditLine", "Alpha Flags:", nullptr));
        lineEdit_MeshName->setPlaceholderText(QCoreApplication::translate("QAlphaEditLine", "mesh name", nullptr));
        label_3->setText(QCoreApplication::translate("QAlphaEditLine", "Flag Value:", nullptr));
        label_2->setText(QCoreApplication::translate("QAlphaEditLine", "Known values:", nullptr));
        label_6->setText(QCoreApplication::translate("QAlphaEditLine", "Part(MesH) Name", nullptr));
        label_4->setText(QCoreApplication::translate("QAlphaEditLine", "Shader name:", nullptr));
        lineEdit_ShaderName->setPlaceholderText(QCoreApplication::translate("QAlphaEditLine", "shader name", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QAlphaEditLine: public Ui_QAlphaEditLine {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QALPHAEDITLINE_H
