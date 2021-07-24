#include "QTextureEditLine.h"

#include "..\ImporterLib\RigidModelFileStructs.h"
#include <qlineedit.h>

QTextureEditLine::QTextureEditLine(QWidget* parent, std::string* _pstrTexturePath, ETextureType* _pETextureType)
	: QWidget(parent)
	, m_pstrTexturePath(_pstrTexturePath)
	, m_pETextureType(_pETextureType)
{
	setupUi(this);

	toolButton->setVisible(true);

	for (uint32_t i = 0; i < 31; i++)
	{
		combonTextureType->addItem(TextureType::getStringFromId(i), i);
	}
	this->toolButton->setText("");
	
	comboBox_TexturePath->lineEdit()->setText( m_pstrTexturePath->c_str() );

	//comboBox_TexturePath->setCursorPosition(0);

	// find the eNum in the control, by converting it to uint32
	QVariant qv = (uint32_t)*_pETextureType;
	int index = combonTextureType->findData(qv);
	combonTextureType->setCurrentIndex(index);
		

	makeConnections();

	toolButton->setIcon(QIcon(":/Icons/dot_menu.png"));

	connect(toolButton, &QToolButton::clicked, toolButton, &QToolButton::showMenu);
	

}

QTextureEditLine::~QTextureEditLine()
{
	int debug_1 = 1;
}

std::pair<std::string*, ETextureType*> QTextureEditLine::getTexture()
{
	return std::pair<std::string*, ETextureType*>(m_pstrTexturePath, m_pETextureType);
}

void QTextureEditLine::setTexture(std::pair<std::string*, ETextureType*> _pair)
{
	*m_pstrTexturePath = *_pair.first;
	*m_pETextureType = *_pair.second;
}

void QTextureEditLine::makeConnections()
{
	connect(comboBox_TexturePath->lineEdit(), &QLineEdit::editingFinished, this, &QTextureEditLine::onTextEditingFinshed);
	connect(comboBox_TexturePath->lineEdit(), &QLineEdit::textChanged, this, &QTextureEditLine::onTextEditingFinshed);
	connect(toolButton, &QToolButton::clicked, this, [this](bool)
		{
			this->gridLayout->addWidget(new QtSearchBox(this), 1, 2);
		}
		
		
		);
}

//void QTextureEditLine::bindToData(std::string* _strTexturePath, ETextureType* _pETextureType)
//{
//	m_pstrTexturePath = _strTexturePath;
//	m_pETextureType = _pETextureType;
//
//	lineEdit->setText(m_pstrTexturePath->c_str());
//	lineEdit->setCursorPosition(0);
//	// find the eNum in the control, by converting it to uint32
//	QVariant qv = (uint32_t)*_pETextureType;
//	int index = combonTextureType->findData(qv);
//	combonTextureType->setCurrentIndex(index);
//}

void QTextureEditLine::onTextEditingFinshed()
{
	if (m_pstrTexturePath)
	{
		*m_pstrTexturePath = comboBox_TexturePath->lineEdit()->text().toStdString();

		auto debug_1 = 1;
	}
}