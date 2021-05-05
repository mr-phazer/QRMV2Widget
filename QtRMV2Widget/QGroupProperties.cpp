#include "QGroupProperties.h"

QGroupPropertiesPanel::QGroupPropertiesPanel(
	QWidget* parent,
	uint32_t* _pdwVertexCount,
	uint32_t* _pdwIndexCount,
	ERigidMaterial* _pRigidMaterial,
	EVertexFormat* _pVertexFormatId)
	:
	QWidget(parent),
	m_pdwVertexCount(_pdwVertexCount),
	m_pdwIndexCount(_pdwIndexCount),
	m_pRigidMaterial(_pRigidMaterial),
	m_pVertexFormatId(_pVertexFormatId)
{
	setupUi(this);

	setupControls();
	doConnect();
}

QGroupPropertiesPanel::~QGroupPropertiesPanel()
{
	int debug_a = 1;
}

void QGroupPropertiesPanel::setupControls()
{
	for (uint32_t i = 0; i < 100; i++)
	{
		this->comboBox_RigidMaterial_Name->addItem(
			getStringFrom_RigidMaterialId((ERigidMaterial)i).c_str(),
			i
		);
	}

	comboBox_RigidMaterial_Name->addItem("Error!", (uint32_t)20000);
	comboBox_RigidMaterial_Name->setCurrentIndex(comboBox_VertexFormat_Name->findData((uint32_t)20000));

	this->comboBox_VertexFormat_Name->clear();
	this->comboBox_VertexFormat_Name->addItem("Standard (static)", (uint32_t)0);
	this->comboBox_VertexFormat_Name->addItem("Weighted 2w/v", (uint32_t)3);
	this->comboBox_VertexFormat_Name->addItem("Cinematic 4w/v", (uint32_t)4);

	comboBox_VertexFormat_Name->addItem("Error!", (uint32_t)20000);
	comboBox_VertexFormat_Name->setCurrentIndex(comboBox_VertexFormat_Name->findData((uint32_t)20000));
}

//void QGroupProperties::bindToData(uint32_t* _pdwVertexCount, uint32_t* _pdwIndexCount, ERigidMaterial* _pRigidMaterial, VertexFormat::EVertexType* _pVertexFormatId)
//{
//	m_pdwVertexCount = _pdwVertexCount;
//	m_pdwIndexCount = _pdwIndexCount;
//	m_pRigidMaterial = _pRigidMaterial;
//	m_pVertexFormatId = _pVertexFormatId;
//}

void QGroupPropertiesPanel::doConnect()
{
	// Rigid Material Combo Box
	connect(lineEdit_RigidMaterial_Id, &QLineEdit::textEdited, this,
		[this](const QString& qstr)
		{
			if (m_pRigidMaterial)
			{
				uint32_t uiData = qstr.toUInt();

				// store in data;
				*m_pRigidMaterial = (ERigidMaterial)uiData;

				int index = comboBox_RigidMaterial_Name->findData(uiData);

				if (index == -1)
					comboBox_RigidMaterial_Name->setCurrentIndex(
					comboBox_RigidMaterial_Name->findData(20000)
					);
				else
					comboBox_RigidMaterial_Name->setCurrentIndex(
					index
					);
			}
		}
	);

	// Rigid_Material Line Edit
	connect(comboBox_RigidMaterial_Name, qOverload<int>(&QComboBox::activated),
		[this](int index)
		{
			uint32_t data = comboBox_RigidMaterial_Name->itemData(index).toUInt();

			lineEdit_RigidMaterial_Id->setText(std::to_string(data).c_str());

			//if (index == 2) // unknown value
			//	lineEditFlagValue->setText("000");
		}
	);

	connect(lineEdit_VertexFormatId, &QLineEdit::textEdited, this,
		[this](const QString& qstr)
		{
			if (m_pVertexFormatId)
			{
				uint32_t uiData = qstr.toUInt();

				// store in data;
				*m_pVertexFormatId = (EVertexFormat)uiData;

				int index = comboBox_VertexFormat_Name->findData(uiData);

				if (index == -1)
					comboBox_VertexFormat_Name->setCurrentIndex(
					comboBox_VertexFormat_Name->findData(20000)
					);
				else
					comboBox_VertexFormat_Name->setCurrentIndex(
					index
					);
			}
		}
	);

	// Rigid_Material Line Edit
	connect(comboBox_VertexFormat_Name, qOverload<int>(&QComboBox::activated),
		[this](int index)
		{
			uint32_t data = comboBox_VertexFormat_Name->itemData(index).toUInt();

			lineEdit_VertexFormatId->setText(std::to_string(data).c_str());

			//if (index == 2) // unknown value
			//	lineEditFlagValue->setText("000");
		}
	);
}

void QGroupPropertiesPanel::updateControls()
{
	// update vertex count (if pointer is set)
	if (m_pdwVertexCount)
		lineEdit_VertexCount->setText(QString::number(*m_pdwVertexCount));

	// update index/triangle count (if pointer is set)
	if (m_pdwIndexCount)
		lineEdit_TriangleCount->setText(QString::number((*m_pdwIndexCount) / 3));

	// update rigid_material combobox (id lineEdit should update automatically)
	if (m_pRigidMaterial)
	{
		int index = comboBox_RigidMaterial_Name->findData((uint32_t)*m_pRigidMaterial);

		if (index > -1)
		{
			comboBox_RigidMaterial_Name->setCurrentIndex(index);
			emit comboBox_RigidMaterial_Name->activated(index);
		}
	}

	if (m_pVertexFormatId)
	{
		int index = comboBox_VertexFormat_Name->findData((uint32_t)*m_pVertexFormatId);

		if (index > -1)
		{
			comboBox_VertexFormat_Name->setCurrentIndex(index);
			emit comboBox_VertexFormat_Name->activated(index);
		}
	}
}

void QGroupPropertiesPanel::showEvent(QShowEvent* event)
{
	updateControls();
}