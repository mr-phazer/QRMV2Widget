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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QTextureEditLine
{
public:
    QHBoxLayout *horizontalLayout;
    QCheckBox *checkBox;
    QComboBox *combonTextureType;
    QComboBox *comboBox_TexturePath;
    QToolButton *toolButton;

    void setupUi(QWidget *QTextureEditLine)
    {
        if (QTextureEditLine->objectName().isEmpty())
            QTextureEditLine->setObjectName(QString::fromUtf8("QTextureEditLine"));
        QTextureEditLine->resize(919, 55);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QTextureEditLine->sizePolicy().hasHeightForWidth());
        QTextureEditLine->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(QTextureEditLine);
        horizontalLayout->setSpacing(3);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(1, 1, 1, 1);
        checkBox = new QCheckBox(QTextureEditLine);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(checkBox->sizePolicy().hasHeightForWidth());
        checkBox->setSizePolicy(sizePolicy1);
        checkBox->setFocusPolicy(Qt::NoFocus);
        checkBox->setChecked(true);

        horizontalLayout->addWidget(checkBox);

        combonTextureType = new QComboBox(QTextureEditLine);
        combonTextureType->addItem(QString());
        combonTextureType->setObjectName(QString::fromUtf8("combonTextureType"));
        combonTextureType->setEnabled(false);
        combonTextureType->setMinimumSize(QSize(95, 0));
        combonTextureType->setMaximumSize(QSize(70, 16777215));
        combonTextureType->setFocusPolicy(Qt::NoFocus);

        horizontalLayout->addWidget(combonTextureType);

        comboBox_TexturePath = new QComboBox(QTextureEditLine);
        comboBox_TexturePath->setObjectName(QString::fromUtf8("comboBox_TexturePath"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(comboBox_TexturePath->sizePolicy().hasHeightForWidth());
        comboBox_TexturePath->setSizePolicy(sizePolicy2);
        comboBox_TexturePath->setEditable(true);

        horizontalLayout->addWidget(comboBox_TexturePath);

        toolButton = new QToolButton(QTextureEditLine);
        toolButton->setObjectName(QString::fromUtf8("toolButton"));
        toolButton->setMinimumSize(QSize(35, 0));
        toolButton->setMaximumSize(QSize(20, 16777215));
        toolButton->setFocusPolicy(Qt::NoFocus);
        QIcon icon;
        icon.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        toolButton->setIcon(icon);
        toolButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
        toolButton->setAutoRaise(false);
        toolButton->setArrowType(Qt::NoArrow);

        horizontalLayout->addWidget(toolButton);


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
        combonTextureType->setItemText(0, QString());

#if QT_CONFIG(tooltip)
        combonTextureType->setToolTip(QCoreApplication::translate("QTextureEditLine", "Texture Type...", nullptr));
#endif // QT_CONFIG(tooltip)
        combonTextureType->setPlaceholderText(QCoreApplication::translate("QTextureEditLine", "texture type", nullptr));
        toolButton->setText(QCoreApplication::translate("QTextureEditLine", "...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QTextureEditLine: public Ui_QTextureEditLine {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTEXTUREEDITLINE_H