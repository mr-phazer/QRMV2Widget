#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtRMV2Widget.h"

class QtRMV2Widget : public QMainWindow
{
    Q_OBJECT

public:
    QtRMV2Widget(QWidget *parent = Q_NULLPTR);

private:
    Ui::QtRMV2WidgetClass ui;
};
