#include "QAlphaEditLine.h"

void QAlphaEditLine::bindToData(uint32_t* pdwAlpha, uint32_t* pdwMask)
{
	m_pdwAlpha = pdwAlpha;
	m_pdwMask = pdwMask;

	if (m_pdwAlpha)
	{
		int index = comboBoxKnownValues->findData(*m_pdwAlpha);

		comboBoxKnownValues->setCurrentIndex(
			index
		);
	}

}

QAlphaEditLine::QAlphaEditLine(QWidget* parent, uint32_t* pdwAlpha, uint32_t* pdwMask)
	:QWidget(parent)
	, m_pdwAlpha(pdwAlpha)
	, m_pdwMask(pdwMask)	
{
	setupUi(this);

	// add the known alpha flag 32 bit values to the combo box.
	comboBoxKnownValues->addItem("Alpha off", (uint32_t)0);
	comboBoxKnownValues->addItem("Alpha on", (uint32_t)1);
	comboBoxKnownValues->addItem("Uknown Value", (uint32_t)20000);
	comboBoxKnownValues->addItem("Not Set: Error!", (uint32_t)10000);
	
	comboBoxKnownValues->setCurrentIndex(
		comboBoxKnownValues->findData(10000)
	);
	
	lineEditFlagValue->setText(std::to_string(*pdwAlpha).c_str());

	//connect(comboBoxKnownValues, &QComboBox::textActivated, this,  &QAlphaEditLine::onTextActivated);
	connect(lineEditFlagValue, &QLineEdit::textEdited, this,
		[this](const QString& qstr)
		{
			if (m_pdwAlpha)
			{				
				*m_pdwAlpha = qstr.toUInt();

				int index = comboBoxKnownValues->findData(*m_pdwAlpha);

				if (index == -1)
					comboBoxKnownValues->setCurrentIndex(
						comboBoxKnownValues->findData(20000)
					);
				else
				comboBoxKnownValues->setCurrentIndex(
					index
				);

			

			}
	
		}
	);
	
	//connect(comboBoxKnownValues, qOverload<int>(&QComboBox::activated),  this,  &QAlphaEditLine::onActivated);
	
	// handle "pick new value in combobox" event
	connect(comboBoxKnownValues, qOverload<int>(&QComboBox::activated),
		[this](int index)
		{
			uint32_t data = comboBoxKnownValues->itemData(index).toUInt();
			lineEditFlagValue->setText(std::to_string(data).c_str());		

			if (index == 2) // unknown value
				lineEditFlagValue->setText("000");
		}	
	);


	if (m_pdwAlpha)
	{
		int index = comboBoxKnownValues->findData(*m_pdwAlpha);

		comboBoxKnownValues->setCurrentIndex(
			index
		);
	}

	
}

QAlphaEditLine::~QAlphaEditLine()
{
}

void QAlphaEditLine::onTextActivated(const QString& text)
{
	
	int debug_1 = 10;

}

void QAlphaEditLine::onActivated(int index)
{
	uint32_t data = comboBoxKnownValues->itemData(index).toUInt();
	lineEditFlagValue->setText(std::to_string(data).c_str());
	int debug_1 = 10;

}
