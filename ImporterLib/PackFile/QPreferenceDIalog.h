#pragma once

#include <QDialog>
#include "ui_QPreferenceDIalog.h"

class QPreferenceDIalog : public QDialog, public Ui::Dialog_Preference
{
	Q_OBJECT

public:
	QPreferenceDIalog(QWidget *parent = Q_NULLPTR);
	~QPreferenceDIalog();
};
