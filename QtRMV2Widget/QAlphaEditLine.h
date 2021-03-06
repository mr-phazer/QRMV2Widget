#pragma once

#include <QWidget>
#include "ui_QAlphaEditLine.h"
#include <string>

class QAlphaEditLine : public QWidget, public Ui::QAlphaEditLine
{
	Q_OBJECT

public:
	void bindToData(uint32_t* pdwAlpha, uint32_t* pdwMask);

	QAlphaEditLine(QWidget* parent, uint32_t* pdwAlpha, uint32_t* pdwMask);
	~QAlphaEditLine();

	uint32_t getAlpha() {
		return *m_pdwAlpha;
	}

	void setAlpha(uint32_t _alpha)
	{
		*m_pdwAlpha = _alpha;
		lineEditFlagValue->setText(std::to_string(_alpha).c_str());
		emit lineEditFlagValue->textEdited(std::to_string(_alpha).c_str());
	}

protected:
	uint32_t* m_pdwAlpha = nullptr;
	uint32_t* m_pdwMask = nullptr;

private slots:
	void onTextActivated(const QString& text);
	void onActivated(int index);
};
