/********************************************************************************
** Form generated from reading UI file 'QtRMV2Widget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTRMV2WIDGET_H
#define UI_QTRMV2WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtRMV2WidgetClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QtRMV2WidgetClass)
    {
        if (QtRMV2WidgetClass->objectName().isEmpty())
            QtRMV2WidgetClass->setObjectName(QString::fromUtf8("QtRMV2WidgetClass"));
        QtRMV2WidgetClass->resize(600, 400);
        menuBar = new QMenuBar(QtRMV2WidgetClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        QtRMV2WidgetClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QtRMV2WidgetClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        QtRMV2WidgetClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(QtRMV2WidgetClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        QtRMV2WidgetClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(QtRMV2WidgetClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        QtRMV2WidgetClass->setStatusBar(statusBar);

        retranslateUi(QtRMV2WidgetClass);

        QMetaObject::connectSlotsByName(QtRMV2WidgetClass);
    } // setupUi

    void retranslateUi(QMainWindow *QtRMV2WidgetClass)
    {
        QtRMV2WidgetClass->setWindowTitle(QCoreApplication::translate("QtRMV2WidgetClass", "QtRMV2Widget", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtRMV2WidgetClass: public Ui_QtRMV2WidgetClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTRMV2WIDGET_H
