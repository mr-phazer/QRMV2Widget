#include "QMaterialEditView.h"

QMaterialEditView::QMaterialEditView(QWidget* parent)
	: QWidget(parent)
{
	setupUi(this);
}

QMaterialEditView::~QMaterialEditView()
{
}

QTextureEditLine* QMaterialEditView::addTextureLine(size_t _posistion, std::string* _pstrTexturePath, ETextureType* _pETextureType)
{
	//// add texture edit line to the material view widget
	auto pTextureEditLine = new QTextureEditLine(
		this,
		_pstrTexturePath,
		_pETextureType
	);

	verticalLayout->insertWidget(_posistion, pTextureEditLine);

	this->vecpTextureEditLine.push_back(pTextureEditLine);

	return  pTextureEditLine;
}

//QTextureEditLine* QMaterialEditView::addTextureEditLine(int i)
//{
//
//	QTextureEditLine* p = new QTextureEditLine(this);
//
//	// store the pointer for later clean up
//	//m_vecAllocated.push_back(shared_ptr<QObject>(p));
//
//	p->show();
//	this->verticalLayout->insertWidget(i, p);
//
//	return p;
//
//}

//QAlphaEditLine* QMaterialEditView::addAlphaEditLine()
//{
//
//
//	QAlphaEditLine* p = new QAlphaEditLine(this);
//
//
//	// store the pointer for later clean up
//	//m_vecAllocated.push_back(shared_ptr<QObject>(p));
//	p->show();
//
//	this->verticalLayout->insertWidget(-1, p);
//
//
//
//	return nullptr;
//}

vector<pair<string*, ETextureType*>> QMaterialEditView::getTextues()
{
	vector<pair<string*, ETextureType*>> vecTemp(vecpTextureEditLine.size());

	// copy each texture line to a temp texture set
	for (size_t i = 0; i < vecpTextureEditLine.size(); i++)
	{
		vecTemp[i].first = vecpTextureEditLine[i]->getTexture().first;
		vecTemp[i].second = vecpTextureEditLine[i]->getTexture().second;
	}

	// return temp texture set
	return vecTemp;
}

void QMaterialEditView::setTextures(vector<pair<string*, ETextureType*>> vecTextures)
{
	//vecpTextureEditLine.clear();
	//vecpTextureEditLine.resize(vecTextures.size());

	for (size_t i = 0; i < vecpTextureEditLine.size(); i++)
	{
		*vecpTextureEditLine[i]->m_pstrTexturePath = *vecTextures[i].first;
		vecpTextureEditLine[i]->comboBox_TexturePath->lineEdit()->setText((*vecTextures[i].first).c_str());
		//*vecpTextureEditLine[i]->getTexture().second = *vecTextures[i].second;
		vecpTextureEditLine[i]->comboBox_TexturePath->lineEdit()->update();
	}
}

QAlphaEditLine* QMaterialEditView::addAlphaAditLine(
	QWidget* parent, uint32_t* pdwAlpha, uint32_t* pdwMask,
		std::string* pstrShaderName,
		std::string* pstrMeshName,
		std::string* pstrTexFolder)
{
	m_poAlphaEditLine = new QAlphaEditLine(
		parent,
		pdwAlpha,
		pdwMask,
		pstrShaderName,
		pstrMeshName,
		pstrTexFolder);

	return m_poAlphaEditLine;
}

QGroupPropertiesPanel* QMaterialEditView::addGroupPropertiesPanel()
{
	//if (pGroupItem->m_pMaterialView->m_pGroupProperties)
	//{
	//	pGroupItem->m_pMaterialView->m_pGroupProperties->bindToData(
	//		&_poFile->vecLodData[lod].vecGroups[group].oHeader.uiVertextCount,
	//		&_poFile->vecLodData[lod].vecGroups[group].oHeader.uiIndexCount,
	//		&_poFile->vecLodData[lod].vecGroups[group].oHeader.RigidMaterialId,
	//		&_poFile->vecLodData[lod].vecGroups[group].oHeader.VertexFormatId
	//	);

	//}

	return nullptr;
}