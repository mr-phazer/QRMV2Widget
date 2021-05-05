/********************************************************************************
** Form generated from reading UI file 'QRMV2Widget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QRMV2WIDGET_H
#define UI_QRMV2WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QRMV2Widget
{
public:
    QVBoxLayout *verticalLayout_5;
    QSplitter *splitter;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QTreeWidget *treeWidget;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *pushButton_CopyTextureGroup;
    QPushButton *pushButton_CopyAlphaAlphaGroup;
    QCheckBox *checkBox;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout_LeftPanel;
    QVBoxLayout *verticalLayout_TextureList;
    QSpacerItem *verticalSpacer_2;
    QVBoxLayout *verticalLayout_MateterialInfo;
    QVBoxLayout *verticalLayout_GroupInfo;

    void setupUi(QWidget *QRMV2Widget)
    {
        if (QRMV2Widget->objectName().isEmpty())
            QRMV2Widget->setObjectName(QString::fromUtf8("QRMV2Widget"));
        QRMV2Widget->resize(851, 584);
        verticalLayout_5 = new QVBoxLayout(QRMV2Widget);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        splitter = new QSplitter(QRMV2Widget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setLineWidth(23);
        splitter->setMidLineWidth(23);
        splitter->setOrientation(Qt::Horizontal);
        splitter->setOpaqueResize(true);
        splitter->setHandleWidth(6);
        verticalLayoutWidget = new QWidget(splitter);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        treeWidget = new QTreeWidget(verticalLayoutWidget);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(3);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(treeWidget->sizePolicy().hasHeightForWidth());
        treeWidget->setSizePolicy(sizePolicy);
        treeWidget->setMaximumSize(QSize(16777215, 16777215));
        treeWidget->setFrameShape(QFrame::WinPanel);
        treeWidget->setDragDropMode(QAbstractItemView::NoDragDrop);
        treeWidget->setAlternatingRowColors(true);
        treeWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        treeWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        treeWidget->setAnimated(true);
        treeWidget->header()->setCascadingSectionResizes(true);
        treeWidget->header()->setHighlightSections(true);
        treeWidget->header()->setProperty("showSortIndicator", QVariant(true));

        verticalLayout->addWidget(treeWidget);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label = new QLabel(verticalLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy1(QSizePolicy::Ignored, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);

        verticalLayout_2->addWidget(label);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(9);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(9, 9, 9, 9);
        pushButton_CopyTextureGroup = new QPushButton(verticalLayoutWidget);
        pushButton_CopyTextureGroup->setObjectName(QString::fromUtf8("pushButton_CopyTextureGroup"));
        QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(3);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(pushButton_CopyTextureGroup->sizePolicy().hasHeightForWidth());
        pushButton_CopyTextureGroup->setSizePolicy(sizePolicy2);

        horizontalLayout_6->addWidget(pushButton_CopyTextureGroup);

        pushButton_CopyAlphaAlphaGroup = new QPushButton(verticalLayoutWidget);
        pushButton_CopyAlphaAlphaGroup->setObjectName(QString::fromUtf8("pushButton_CopyAlphaAlphaGroup"));
        sizePolicy2.setHeightForWidth(pushButton_CopyAlphaAlphaGroup->sizePolicy().hasHeightForWidth());
        pushButton_CopyAlphaAlphaGroup->setSizePolicy(sizePolicy2);

        horizontalLayout_6->addWidget(pushButton_CopyAlphaAlphaGroup);


        verticalLayout_2->addLayout(horizontalLayout_6);


        verticalLayout->addLayout(verticalLayout_2);

        checkBox = new QCheckBox(verticalLayoutWidget);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setEnabled(true);
        QSizePolicy sizePolicy3(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(30);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(checkBox->sizePolicy().hasHeightForWidth());
        checkBox->setSizePolicy(sizePolicy3);
        checkBox->setChecked(true);
        checkBox->setTristate(false);

        verticalLayout->addWidget(checkBox);

        splitter->addWidget(verticalLayoutWidget);
        groupBox = new QGroupBox(splitter);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        groupBox->setMinimumSize(QSize(0, 0));
        groupBox->setCheckable(false);
        verticalLayout_4 = new QVBoxLayout(groupBox);
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        verticalLayout_LeftPanel = new QVBoxLayout();
        verticalLayout_LeftPanel->setSpacing(6);
        verticalLayout_LeftPanel->setObjectName(QString::fromUtf8("verticalLayout_LeftPanel"));
        verticalLayout_TextureList = new QVBoxLayout();
        verticalLayout_TextureList->setSpacing(6);
        verticalLayout_TextureList->setObjectName(QString::fromUtf8("verticalLayout_TextureList"));
        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_TextureList->addItem(verticalSpacer_2);


        verticalLayout_LeftPanel->addLayout(verticalLayout_TextureList);

        verticalLayout_MateterialInfo = new QVBoxLayout();
        verticalLayout_MateterialInfo->setSpacing(6);
        verticalLayout_MateterialInfo->setObjectName(QString::fromUtf8("verticalLayout_MateterialInfo"));

        verticalLayout_LeftPanel->addLayout(verticalLayout_MateterialInfo);

        verticalLayout_GroupInfo = new QVBoxLayout();
        verticalLayout_GroupInfo->setSpacing(6);
        verticalLayout_GroupInfo->setObjectName(QString::fromUtf8("verticalLayout_GroupInfo"));

        verticalLayout_LeftPanel->addLayout(verticalLayout_GroupInfo);


        verticalLayout_4->addLayout(verticalLayout_LeftPanel);

        splitter->addWidget(groupBox);

        verticalLayout_5->addWidget(splitter);


        retranslateUi(QRMV2Widget);

        QMetaObject::connectSlotsByName(QRMV2Widget);
    } // setupUi

    void retranslateUi(QWidget *QRMV2Widget)
    {
        QRMV2Widget->setWindowTitle(QCoreApplication::translate("QRMV2Widget", "QRMV2Widget", nullptr));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(0, QCoreApplication::translate("QRMV2Widget", "empty_file.rigid_model_v2", nullptr));
#if QT_CONFIG(tooltip)
        treeWidget->setToolTip(QCoreApplication::translate("QRMV2Widget", "This shows the RigidModel File structure.", nullptr));
#endif // QT_CONFIG(tooltip)
        label->setText(QCoreApplication::translate("QRMV2Widget", "Copy selected Groups to CHECKED Groups:", nullptr));
#if QT_CONFIG(tooltip)
        pushButton_CopyTextureGroup->setToolTip(QCoreApplication::translate("QRMV2Widget", "Copies the textures of the SELECTED group to the CHECKED groups", nullptr));
#endif // QT_CONFIG(tooltip)
        pushButton_CopyTextureGroup->setText(QCoreApplication::translate("QRMV2Widget", "Fill Texture Paths", nullptr));
#if QT_CONFIG(tooltip)
        pushButton_CopyAlphaAlphaGroup->setToolTip(QCoreApplication::translate("QRMV2Widget", "Copies the ALPHA flag of the SELECTED group to the CHECKED groups", nullptr));
#endif // QT_CONFIG(tooltip)
        pushButton_CopyAlphaAlphaGroup->setText(QCoreApplication::translate("QRMV2Widget", "Fill Alpha", nullptr));
        checkBox->setText(QCoreApplication::translate("QRMV2Widget", "Show Advanced", nullptr));
        groupBox->setTitle(QCoreApplication::translate("QRMV2Widget", "Material - Textuire Set:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QRMV2Widget: public Ui_QRMV2Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QRMV2WIDGET_H
