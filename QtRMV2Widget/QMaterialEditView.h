#pragma once

#include <QWidget>
#include "ui_QMaterialEditView.h"

#include "QTextureEditLine.h"
#include "QAlphaEditLine.h"
#include "QGroupProperties.h"

#include "..\ImporterLib\\RigidModelV2_Enums.h"

using namespace std;

class QMaterialEditView : public QWidget, public Ui::QMateriaEditView
{
	Q_OBJECT

public:
	QMaterialEditView(QWidget* parent = Q_NULLPTR);
	~QMaterialEditView();

	QTextureEditLine* addTextureLine(
		size_t _posistion,
		std::string* _pstrTexturePath,
		ETextureType* _pETextureType
	);

	void addGroupInfoWidget(size_t t,
		uint32_t* _pdwVertexCount,
		uint32_t* _pdwIndexCount,
		ERigidMaterial* _pRigidMaterial,
		EVertexFormat* _pVertexFormatId
	)
	{
		// add the "group properties" panel
		auto m_pGroupProperties = new QGroupPropertiesPanel(
			this,
			_pdwVertexCount,
			_pdwIndexCount,
			_pRigidMaterial,
			_pVertexFormatId
		);
		// add "group properties panel" to layout
		verticalLayout_GroupInfo->insertWidget(t++, m_pGroupProperties);
	}

	vector< pair<string*, ETextureType* >> getTextues();

	void setTextures(vector< pair<string*, ETextureType* >> vecTextures);

	uint32_t getAlpha()
	{
		if (!m_poAlphaEditLine)
			throw invalid_argument("");;

		return m_poAlphaEditLine->getAlpha();
	}

	void  setAlpha(uint32_t _dwAlpha)
	{
		if (!m_poAlphaEditLine)
			throw invalid_argument("");;

		m_poAlphaEditLine->setAlpha(_dwAlpha);
	}

	QAlphaEditLine* addAlphaAditLine(QWidget* parent, uint32_t* pdwAlpha, uint32_t* pdwMask,
		std::string* pstrShaderName,
		std::string* pstrMeshName,
		std::string* pstrTexFolder);

	QGroupPropertiesPanel* addGroupPropertiesPanel();

	QGroupPropertiesPanel* m_pGroupProperties = nullptr;
private:
	std::vector<shared_ptr<QObject>> m_vecAllocated;

	vector<QTextureEditLine*> vecpTextureEditLine;

	QAlphaEditLine* m_poAlphaEditLine = nullptr;
};
