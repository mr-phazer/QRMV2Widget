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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QTextureEditLine
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    QGridLayout *gridLayout_4;
    QCheckBox *checkBox;
    QToolButton *toolButton;
    QComboBox *combonTextureType;
    QLineEdit *lineEdit;

    void setupUi(QWidget *QTextureEditLine)
    {
        if (QTextureEditLine->objectName().isEmpty())
            QTextureEditLine->setObjectName(QString::fromUtf8("QTextureEditLine"));
        QTextureEditLine->resize(919, 89);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QTextureEditLine->sizePolicy().hasHeightForWidth());
        QTextureEditLine->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(QTextureEditLine);
        horizontalLayout->setSpacing(7);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, -1, 0, -1);
        gridLayout_4 = new QGridLayout();
        gridLayout_4->setSpacing(6);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_4->setSizeConstraint(QLayout::SetDefaultConstraint);
        gridLayout_4->setHorizontalSpacing(5);
        gridLayout_4->setVerticalSpacing(0);
        gridLayout_4->setContentsMargins(4, 0, 4, 0);
        checkBox = new QCheckBox(QTextureEditLine);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setFocusPolicy(Qt::NoFocus);
        checkBox->setChecked(true);

        gridLayout_4->addWidget(checkBox, 0, 0, 1, 1);

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

        gridLayout_4->addWidget(toolButton, 0, 5, 1, 1);

        combonTextureType = new QComboBox(QTextureEditLine);
        combonTextureType->addItem(QString());
        combonTextureType->setObjectName(QString::fromUtf8("combonTextureType"));
        combonTextureType->setEnabled(false);
        combonTextureType->setMinimumSize(QSize(70, 0));
        combonTextureType->setMaximumSize(QSize(70, 16777215));
        combonTextureType->setFocusPolicy(Qt::NoFocus);

        gridLayout_4->addWidget(combonTextureType, 0, 1, 1, 1);

        lineEdit = new QLineEdit(QTextureEditLine);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setMouseTracking(true);
        lineEdit->setMaxLength(255);
        lineEdit->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        lineEdit->setClearButtonEnabled(false);

        gridLayout_4->addWidget(lineEdit, 0, 4, 1, 1);


        verticalLayout_2->addLayout(gridLayout_4);


        horizontalLayout->addLayout(verticalLayout_2);

        QWidget::setTabOrder(combonTextureType, lineEdit);
        QWidget::setTabOrder(lineEdit, checkBox);
        QWidget::setTabOrder(checkBox, toolButton);

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
        toolButton->setText(QString());
        combonTextureType->setItemText(0, QCoreApplication::translate("QTextureEditLine", "123123123123123", nullptr));

#if QT_CONFIG(tooltip)
        combonTextureType->setToolTip(QCoreApplication::translate("QTextureEditLine", "Texture Type...", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        lineEdit->setToolTip(QCoreApplication::translate("QTextureEditLine", "Interal .pack tesxture path (.DDS)", nullptr));
#endif // QT_CONFIG(tooltip)
        lineEdit->setPlaceholderText(QCoreApplication::translate("QTextureEditLine", "enter texture path", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QTextureEditLine: public Ui_QTextureEditLine {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTEXTUREEDITLINE_H
