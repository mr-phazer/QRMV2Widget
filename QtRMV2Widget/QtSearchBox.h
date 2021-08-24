#pragma once

#include <QtWidgets/QWidget>
#include "ui_QtSearchBox.h"

class QtSearchBox : public QWidget, public Ui::QtSearchBox
{
	Q_OBJECT

public:
	QtSearchBox(QWidget* parent = Q_NULLPTR);
	~QtSearchBox();
};
