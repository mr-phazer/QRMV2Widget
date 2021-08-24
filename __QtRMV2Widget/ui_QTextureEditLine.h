/********************************************************************************
** Form generated from reading UI file 'QTextureEditLine.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTEXTUREEDITLINE_H
#define UI_QTEXTUREEDITLINE_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QTextureEditLine
{
public:
    QVBoxLayout *verticalLayout_4;
    QGridLayout *gridLayout;
    QCheckBox *checkBox;
    QComboBox *comboBox_TexturePath;
    QToolButton *toolButton;
    QComboBox *combonTextureType;

    void setupUi(QWidget *QTextureEditLine)
    {
        if (QTextureEditLine->objectName().isEmpty())
            QTextureEditLine->setObjectName(QString::fromUtf8("QTextureEditLine"));
        QTextureEditLine->setEnabled(true);
        QTextureEditLine->resize(776, 30);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QTextureEditLine->sizePolicy().hasHeightForWidth());
        QTextureEditLine->setSizePolicy(sizePolicy);
        QTextureEditLine->setMaximumSize(QSize(16777215, 16777215));
        verticalLayout_4 = new QVBoxLayout(QTextureEditLine);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(1, 1, 1, 1);
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        checkBox = new QCheckBox(QTextureEditLine);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(checkBox->sizePolicy().hasHeightForWidth());
        checkBox->setSizePolicy(sizePolicy1);
        checkBox->setFocusPolicy(Qt::NoFocus);
        checkBox->setChecked(true);

        gridLayout->addWidget(checkBox, 0, 0, 1, 1);

        comboBox_TexturePath = new QComboBox(QTextureEditLine);
        comboBox_TexturePath->setObjectName(QString::fromUtf8("comboBox_TexturePath"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(comboBox_TexturePath->sizePolicy().hasHeightForWidth());
        comboBox_TexturePath->setSizePolicy(sizePolicy2);
        comboBox_TexturePath->setEditable(true);

        gridLayout->addWidget(comboBox_TexturePath, 0, 2, 1, 1);

        toolButton = new QToolButton(QTextureEditLine);
        toolButton->setObjectName(QString::fromUtf8("toolButton"));
        toolButton->setEnabled(true);
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(toolButton->sizePolicy().hasHeightForWidth());
        toolButton->setSizePolicy(sizePolicy3);
        toolButton->setMinimumSize(QSize(0, 0));
        toolButton->setMaximumSize(QSize(16777215, 16777215));
        toolButton->setFocusPolicy(Qt::NoFocus);
        QIcon icon;
        icon.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        toolButton->setIcon(icon);
        toolButton->setPopupMode(QToolButton::InstantPopup);
        toolButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
        toolButton->setAutoRaise(false);
        toolButton->setArrowType(Qt::DownArrow);

        gridLayout->addWidget(toolButton, 0, 3, 1, 1);

        combonTextureType = new QComboBox(QTextureEditLine);
        combonTextureType->addItem(QString());
        combonTextureType->setObjectName(QString::fromUtf8("combonTextureType"));
        combonTextureType->setEnabled(false);
        QSizePolicy sizePolicy4(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(combonTextureType->sizePolicy().hasHeightForWidth());
        combonTextureType->setSizePolicy(sizePolicy4);
        combonTextureType->setMinimumSize(QSize(110, 0));
        combonTextureType->setMaximumSize(QSize(70, 16777215));
        combonTextureType->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(combonTextureType, 0, 1, 1, 1);


        verticalLayout_4->addLayout(gridLayout);


        retranslateUi(QTextureEditLine);

        QMetaObject::connectSlotsByName(QTextureEditLine);
    } // setupUi

    void retranslateUi(QWidget *QTextureEditLine)
    {
        QTextureEditLine->setWindowTitle(QCoreApplication::translate("QTextureEditLine", "QTextureEditLine", nullptr));
#if QT_CONFIG(tooltip)
        checkBox->setToolTip(QCoreApplication::translate("QTextureEditLine", "Enable texture type editing", nullptr));
#endif // QT_CONFIG(tooltip)
        checkBox->setText(QString());
        toolButton->setText(QCoreApplication::translate("QTextureEditLine", "CLiek", nullptr));
        combonTextureType->setItemText(0, QString());

#if QT_CONFIG(tooltip)
        combonTextureType->setToolTip(QCoreApplication::translate("QTextureEditLine", "Texture Type...", nullptr));
#endif // QT_CONFIG(tooltip)
        combonTextureType->setPlaceholderText(QCoreApplication::translate("QTextureEditLine", "texture type", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QTextureEditLine: public Ui_QTextureEditLine {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTEXTUREEDITLINE_H