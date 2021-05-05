/********************************************************************************
** Form generated from reading UI file 'QMaterialEditView.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QMATERIALEDITVIEW_H
#define UI_QMATERIALEDITVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QMateriaEditView
{
public:
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *verticalLayout_Alpha;
    QSpacerItem *verticalSpacer_2;
    QVBoxLayout *verticalLayout_GroupInfo;
    QSpacerItem *verticalSpacer_3;

    void setupUi(QWidget *QMateriaEditView)
    {
        if (QMateriaEditView->objectName().isEmpty())
            QMateriaEditView->setObjectName(QString::fromUtf8("QMateriaEditView"));
        QMateriaEditView->resize(829, 495);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QMateriaEditView->sizePolicy().hasHeightForWidth());
        QMateriaEditView->setSizePolicy(sizePolicy);
        verticalLayout_3 = new QVBoxLayout(QMateriaEditView);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        verticalLayout_2->addLayout(verticalLayout);

        verticalLayout_Alpha = new QVBoxLayout();
        verticalLayout_Alpha->setObjectName(QString::fromUtf8("verticalLayout_Alpha"));
        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_Alpha->addItem(verticalSpacer_2);


        verticalLayout_2->addLayout(verticalLayout_Alpha);

        verticalLayout_GroupInfo = new QVBoxLayout();
        verticalLayout_GroupInfo->setObjectName(QString::fromUtf8("verticalLayout_GroupInfo"));
        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_GroupInfo->addItem(verticalSpacer_3);


        verticalLayout_2->addLayout(verticalLayout_GroupInfo);


        verticalLayout_3->addLayout(verticalLayout_2);


        retranslateUi(QMateriaEditView);

        QMetaObject::connectSlotsByName(QMateriaEditView);
    } // setupUi

    void retranslateUi(QWidget *QMateriaEditView)
    {
        QMateriaEditView->setWindowTitle(QCoreApplication::translate("QMateriaEditView", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QMateriaEditView: public Ui_QMateriaEditView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QMATERIALEDITVIEW_H
