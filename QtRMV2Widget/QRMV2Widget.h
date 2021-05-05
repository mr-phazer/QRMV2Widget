#pragma once

#include <memory>

#include <QWidget>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qtextedit.h>
#include <QtWidgets/qFrame.h>
#include <QtWidgets/qstackedwidget.h>
#include <QtWidgets/qmessagebox.h>

#include "ui_QRMV2Widget.h"

#include "..\ImporterLib\RigidModelFileStructs.h"
#include "QTreeMaterialViewItem.h"
#include "QAlphaEditLine.h"

#include "QGroupProperties.h"


using namespace std;




class QRMV2Widget : public QWidget, public Ui::QRMV2Widget
{
	Q_OBJECT

public:
	QRMV2Widget(QWidget *parent = Q_NULLPTR);
	~QRMV2Widget();

	bool setFile(ImporterLib::RigidModel::IRigidModelFile*);

private slots:
	void onItemClicked(QTreeWidgetItem* item, int column);
	void onGroupCopy_Texture_ButtonClicked();
	//void onGroupCopy_Alpha_ButtonClicked();

public:
	QStackedWidget* m_pPStackedWidget = nullptr;
	void insertFileHeader(ImporterLib::RigidModel::IRigidModelFile*, QTreeWidgetItem* m_poParent);
	void insertLODHeaders(ImporterLib::RigidModel::IRigidModelFile*, QTreeWidgetItem* m_poParent);
	
	QTreeMaterialViewItem* createMaterialViewTreeItem(QTreeWidgetItem* pLodItem, const std::string& strGroupName)
	{
		QTreeMaterialViewItem* pGroupItem = new QTreeMaterialViewItem(
			{ QString(strGroupName.c_str()) });

		pGroupItem->setFlags(pGroupItem->flags() | Qt::ItemIsUserCheckable);
		pGroupItem->setCheckState(0, Qt::CheckState::Unchecked);

		// add group item to LOD parent
		pLodItem->addChild(pGroupItem);

		pLodItem->setExpanded(true);

		vecMaterialViewsItems.push_back(pGroupItem);		

		return pGroupItem;
	}



	QTreeWidgetItem* insertLODNode(QTreeWidgetItem* pTopLevel, size_t lod);

	
protected:
	vector<QTreeMaterialViewItem*> vecMaterialViewsItems;




};


