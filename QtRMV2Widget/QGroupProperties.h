#pragma once

#include <QtWidgets/QWidget>
#include "ui_QGroupProperties.h"
#include "..\ImporterLib\IRigidModelmporter.h"
#include "..\ImporterLib\RigidModelV2_Enums.h"

class QGroupPropertiesPanel : public QWidget, public Ui::QGroupProperties
{
	Q_OBJECT

private:
	//QGroupProperties() {};

public:
	QGroupPropertiesPanel(
		QWidget* parent,
		uint32_t* _pdwVertexCount,
		uint32_t* _pdwIndexCount,
		ERigidMaterial* _pRigidMaterial,
		EVertexFormat* _pVertexFormatId);

	~QGroupPropertiesPanel();

	void setupControls();
	/// <summary>
	/// Store pointers to the data the controls are editing
	/// </summary>
	/// <param name="_pdwVertexCount"></param>
	/// <param name="_pdwIndexCount"></param>
	/// <param name="_pRigidMaterial"></param>
	/// <param name="_pVertexFormatId"></param>
	//void bindToData(
	//	uint32_t* _pdwVertexCount,
	//	uint32_t* _pdwIndexCount,
	//	ERigidMaterial* _pRigidMaterial,
	//	VertexFormat::EVertexType* _pVertexFormatId
	//);

	void doConnect();

	void updateControls();
	void updateData();

	virtual void showEvent(QShowEvent* event) override;

private:
	uint32_t* m_pdwVertexCount = nullptr;
	uint32_t* m_pdwIndexCount = nullptr;
	ERigidMaterial* m_pRigidMaterial = nullptr;
	EVertexFormat* m_pVertexFormatId = nullptr;
};
