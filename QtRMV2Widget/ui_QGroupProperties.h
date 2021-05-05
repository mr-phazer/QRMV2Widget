/********************************************************************************
** Form generated from reading UI file 'QGroupProperties.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QGROUPPROPERTIES_H
#define UI_QGROUPPROPERTIES_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QGroupProperties
{
public:
    QHBoxLayout *horizontalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *groupBox_GeometryInfo;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout_GeomtryInfo;
    QLabel *label_TriangleCount;
    QLabel *label_VertexCount;
    QLineEdit *lineEdit_VertexCount;
    QLineEdit *lineEdit_TriangleCount;
    QGroupBox *groupBox_VertexFormat;
    QVBoxLayout *verticalLayout_3;
    QGridLayout *gridLayout_RigidMaterial_2;
    QLineEdit *lineEdit_VertexFormatId;
    QComboBox *comboBox_VertexFormat_Name;
    QLabel *label_6;
    QLabel *label_5;
    QGroupBox *groupBox_RigidMaterial;
    QVBoxLayout *verticalLayout_2;
    QGridLayout *gridLayout_RigidMaterial;
    QLineEdit *lineEdit_RigidMaterial_Id;
    QComboBox *comboBox_RigidMaterial_Name;
    QLabel *label_3;
    QLabel *label_4;

    void setupUi(QWidget *QGroupProperties)
    {
        if (QGroupProperties->objectName().isEmpty())
            QGroupProperties->setObjectName(QString::fromUtf8("QGroupProperties"));
        QGroupProperties->resize(618, 300);
        horizontalLayout_3 = new QHBoxLayout(QGroupProperties);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        groupBox_GeometryInfo = new QGroupBox(QGroupProperties);
        groupBox_GeometryInfo->setObjectName(QString::fromUtf8("groupBox_GeometryInfo"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(6);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox_GeometryInfo->sizePolicy().hasHeightForWidth());
        groupBox_GeometryInfo->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(groupBox_GeometryInfo);
        verticalLayout->setSpacing(2);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(2, 2, 2, 2);
        gridLayout_GeomtryInfo = new QGridLayout();
        gridLayout_GeomtryInfo->setSpacing(6);
        gridLayout_GeomtryInfo->setObjectName(QString::fromUtf8("gridLayout_GeomtryInfo"));
        gridLayout_GeomtryInfo->setContentsMargins(3, 3, 3, 3);
        label_TriangleCount = new QLabel(groupBox_GeometryInfo);
        label_TriangleCount->setObjectName(QString::fromUtf8("label_TriangleCount"));

        gridLayout_GeomtryInfo->addWidget(label_TriangleCount, 1, 0, 1, 1);

        label_VertexCount = new QLabel(groupBox_GeometryInfo);
        label_VertexCount->setObjectName(QString::fromUtf8("label_VertexCount"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_VertexCount->sizePolicy().hasHeightForWidth());
        label_VertexCount->setSizePolicy(sizePolicy1);

        gridLayout_GeomtryInfo->addWidget(label_VertexCount, 0, 0, 1, 1);

        lineEdit_VertexCount = new QLineEdit(groupBox_GeometryInfo);
        lineEdit_VertexCount->setObjectName(QString::fromUtf8("lineEdit_VertexCount"));
        lineEdit_VertexCount->setEnabled(true);
        QSizePolicy sizePolicy2(QSizePolicy::Ignored, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(1);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(lineEdit_VertexCount->sizePolicy().hasHeightForWidth());
        lineEdit_VertexCount->setSizePolicy(sizePolicy2);
        lineEdit_VertexCount->setReadOnly(true);

        gridLayout_GeomtryInfo->addWidget(lineEdit_VertexCount, 0, 1, 1, 1);

        lineEdit_TriangleCount = new QLineEdit(groupBox_GeometryInfo);
        lineEdit_TriangleCount->setObjectName(QString::fromUtf8("lineEdit_TriangleCount"));
        lineEdit_TriangleCount->setEnabled(true);
        sizePolicy2.setHeightForWidth(lineEdit_TriangleCount->sizePolicy().hasHeightForWidth());
        lineEdit_TriangleCount->setSizePolicy(sizePolicy2);
        lineEdit_TriangleCount->setReadOnly(true);

        gridLayout_GeomtryInfo->addWidget(lineEdit_TriangleCount, 1, 1, 1, 1);


        verticalLayout->addLayout(gridLayout_GeomtryInfo);


        horizontalLayout_2->addWidget(groupBox_GeometryInfo);

        groupBox_VertexFormat = new QGroupBox(QGroupProperties);
        groupBox_VertexFormat->setObjectName(QString::fromUtf8("groupBox_VertexFormat"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(10);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(groupBox_VertexFormat->sizePolicy().hasHeightForWidth());
        groupBox_VertexFormat->setSizePolicy(sizePolicy3);
        verticalLayout_3 = new QVBoxLayout(groupBox_VertexFormat);
        verticalLayout_3->setSpacing(2);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(2, 2, 2, 2);
        gridLayout_RigidMaterial_2 = new QGridLayout();
        gridLayout_RigidMaterial_2->setSpacing(2);
        gridLayout_RigidMaterial_2->setObjectName(QString::fromUtf8("gridLayout_RigidMaterial_2"));
        gridLayout_RigidMaterial_2->setContentsMargins(3, 3, -1, 3);
        lineEdit_VertexFormatId = new QLineEdit(groupBox_VertexFormat);
        lineEdit_VertexFormatId->setObjectName(QString::fromUtf8("lineEdit_VertexFormatId"));
        lineEdit_VertexFormatId->setEnabled(true);
        QSizePolicy sizePolicy4(QSizePolicy::Ignored, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(2);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(lineEdit_VertexFormatId->sizePolicy().hasHeightForWidth());
        lineEdit_VertexFormatId->setSizePolicy(sizePolicy4);
        lineEdit_VertexFormatId->setAlignment(Qt::AlignBottom|Qt::AlignLeading|Qt::AlignLeft);
        lineEdit_VertexFormatId->setReadOnly(true);

        gridLayout_RigidMaterial_2->addWidget(lineEdit_VertexFormatId, 1, 1, 1, 1);

        comboBox_VertexFormat_Name = new QComboBox(groupBox_VertexFormat);
        comboBox_VertexFormat_Name->addItem(QString());
        comboBox_VertexFormat_Name->setObjectName(QString::fromUtf8("comboBox_VertexFormat_Name"));
        comboBox_VertexFormat_Name->setEnabled(false);
        QSizePolicy sizePolicy5(QSizePolicy::Ignored, QSizePolicy::Fixed);
        sizePolicy5.setHorizontalStretch(4);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(comboBox_VertexFormat_Name->sizePolicy().hasHeightForWidth());
        comboBox_VertexFormat_Name->setSizePolicy(sizePolicy5);

        gridLayout_RigidMaterial_2->addWidget(comboBox_VertexFormat_Name, 1, 0, 1, 1);

        label_6 = new QLabel(groupBox_VertexFormat);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        QSizePolicy sizePolicy6(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy6);
        label_6->setAlignment(Qt::AlignBottom|Qt::AlignLeading|Qt::AlignLeft);

        gridLayout_RigidMaterial_2->addWidget(label_6, 0, 1, 1, 1);

        label_5 = new QLabel(groupBox_VertexFormat);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        sizePolicy6.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy6);
        label_5->setAlignment(Qt::AlignBottom|Qt::AlignLeading|Qt::AlignLeft);

        gridLayout_RigidMaterial_2->addWidget(label_5, 0, 0, 1, 1);


        verticalLayout_3->addLayout(gridLayout_RigidMaterial_2);


        horizontalLayout_2->addWidget(groupBox_VertexFormat);

        groupBox_RigidMaterial = new QGroupBox(QGroupProperties);
        groupBox_RigidMaterial->setObjectName(QString::fromUtf8("groupBox_RigidMaterial"));
        sizePolicy3.setHeightForWidth(groupBox_RigidMaterial->sizePolicy().hasHeightForWidth());
        groupBox_RigidMaterial->setSizePolicy(sizePolicy3);
        verticalLayout_2 = new QVBoxLayout(groupBox_RigidMaterial);
        verticalLayout_2->setSpacing(2);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(2, 2, 2, 2);
        gridLayout_RigidMaterial = new QGridLayout();
        gridLayout_RigidMaterial->setSpacing(2);
        gridLayout_RigidMaterial->setObjectName(QString::fromUtf8("gridLayout_RigidMaterial"));
        gridLayout_RigidMaterial->setContentsMargins(3, 3, 3, 3);
        lineEdit_RigidMaterial_Id = new QLineEdit(groupBox_RigidMaterial);
        lineEdit_RigidMaterial_Id->setObjectName(QString::fromUtf8("lineEdit_RigidMaterial_Id"));
        lineEdit_RigidMaterial_Id->setEnabled(true);
        sizePolicy4.setHeightForWidth(lineEdit_RigidMaterial_Id->sizePolicy().hasHeightForWidth());
        lineEdit_RigidMaterial_Id->setSizePolicy(sizePolicy4);
        lineEdit_RigidMaterial_Id->setAlignment(Qt::AlignBottom|Qt::AlignLeading|Qt::AlignLeft);
        lineEdit_RigidMaterial_Id->setReadOnly(true);

        gridLayout_RigidMaterial->addWidget(lineEdit_RigidMaterial_Id, 1, 1, 1, 1);

        comboBox_RigidMaterial_Name = new QComboBox(groupBox_RigidMaterial);
        comboBox_RigidMaterial_Name->addItem(QString());
        comboBox_RigidMaterial_Name->setObjectName(QString::fromUtf8("comboBox_RigidMaterial_Name"));
        comboBox_RigidMaterial_Name->setEnabled(false);
        sizePolicy5.setHeightForWidth(comboBox_RigidMaterial_Name->sizePolicy().hasHeightForWidth());
        comboBox_RigidMaterial_Name->setSizePolicy(sizePolicy5);
        comboBox_RigidMaterial_Name->setMaxVisibleItems(1);

        gridLayout_RigidMaterial->addWidget(comboBox_RigidMaterial_Name, 1, 0, 1, 1);

        label_3 = new QLabel(groupBox_RigidMaterial);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        sizePolicy6.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy6);
        label_3->setAlignment(Qt::AlignBottom|Qt::AlignLeading|Qt::AlignLeft);

        gridLayout_RigidMaterial->addWidget(label_3, 0, 0, 1, 1);

        label_4 = new QLabel(groupBox_RigidMaterial);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        sizePolicy6.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy6);
        label_4->setAlignment(Qt::AlignBottom|Qt::AlignLeading|Qt::AlignLeft);

        gridLayout_RigidMaterial->addWidget(label_4, 0, 1, 1, 1);


        verticalLayout_2->addLayout(gridLayout_RigidMaterial);


        horizontalLayout_2->addWidget(groupBox_RigidMaterial);


        horizontalLayout_3->addLayout(horizontalLayout_2);


        retranslateUi(QGroupProperties);

        QMetaObject::connectSlotsByName(QGroupProperties);
    } // setupUi

    void retranslateUi(QWidget *QGroupProperties)
    {
        QGroupProperties->setWindowTitle(QCoreApplication::translate("QGroupProperties", "QMaterialEditorWidget", nullptr));
        groupBox_GeometryInfo->setTitle(QCoreApplication::translate("QGroupProperties", "Geometry Info:", nullptr));
        label_TriangleCount->setText(QCoreApplication::translate("QGroupProperties", "Triangles:", nullptr));
        label_VertexCount->setText(QCoreApplication::translate("QGroupProperties", "Vertices: ", nullptr));
        lineEdit_VertexCount->setText(QCoreApplication::translate("QGroupProperties", "error!", nullptr));
        lineEdit_TriangleCount->setText(QCoreApplication::translate("QGroupProperties", "error!", nullptr));
        groupBox_VertexFormat->setTitle(QCoreApplication::translate("QGroupProperties", "Vertex Format:", nullptr));
        lineEdit_VertexFormatId->setText(QCoreApplication::translate("QGroupProperties", "error!", nullptr));
        comboBox_VertexFormat_Name->setItemText(0, QCoreApplication::translate("QGroupProperties", "empty: error!", nullptr));

        label_6->setText(QCoreApplication::translate("QGroupProperties", "Id:", nullptr));
        label_5->setText(QCoreApplication::translate("QGroupProperties", "Name:", nullptr));
        groupBox_RigidMaterial->setTitle(QCoreApplication::translate("QGroupProperties", "Rigid_Material", nullptr));
        lineEdit_RigidMaterial_Id->setText(QCoreApplication::translate("QGroupProperties", "error!", nullptr));
        comboBox_RigidMaterial_Name->setItemText(0, QCoreApplication::translate("QGroupProperties", "empty: error!", nullptr));

        label_3->setText(QCoreApplication::translate("QGroupProperties", "Name:", nullptr));
        label_4->setText(QCoreApplication::translate("QGroupProperties", "Id:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QGroupProperties: public Ui_QGroupProperties {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QGROUPPROPERTIES_H
