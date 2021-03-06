#pragma once

#include <memory>

#include <QtWidgets/qwidget.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qtextedit.h>
#include <QtWidgets/qFrame.h>
#include <QtWidgets/qstackedwidget.h>
#include <QtWidgets/qmessagebox.h>

#include "ui_QRMV2Widget.h"

#include "..\ImporterLib\RigidModelFileStructs.h"
#include "QTreeMaterialViewItem.h"
#include "QAlphaEditLine.h"

#include "..\ImporterLib\RigidModelV2_Reader.h"
#include "..\ImporterLib\RigidModelV2_Writer.h"

#include "QGroupProperties.h"

using namespace std;

class QRMV2Widget : public QWidget, public Ui::QRMV2Widget
{
	Q_OBJECT

public:
	QRMV2Widget(QWidget* parent = Q_NULLPTR);
	~QRMV2Widget();

	bool setFile(ImporterLib::RigidModel::IRigidModelFile*);
	bool setFile_New(shared_ptr<RigidModelV2::Common::CommonFile> _spoCommonFile);

private slots:
	void onItemClicked(QTreeWidgetItem* item, int column);
	void onItemSelectionChange();

	void onGroupCopy_Texture_ButtonClicked();
	void onGroupCopy_Alpha_ButtonClicked();

private:
	QStackedWidget* m_pPStackedWidget = nullptr;
	void insertFileHeader(ImporterLib::RigidModel::IRigidModelFile*, QTreeWidgetItem* _poParent);
	void insertLODHeaders(ImporterLib::RigidModel::IRigidModelFile*, QTreeWidgetItem* _poParent);

	void insertFileHeader_New(QTreeWidgetItem* _poParent);
	void insertLODHeaders_New(QTreeWidgetItem* _poParent);

	/*QString lastErrorString();
	static QString lastGlobalErrorString();*/

	QTreeMaterialViewItem* createMaterialViewTreeItem(QTreeWidgetItem* pLodItem, const std::string& strGroupName);

	QTreeWidgetItem* insertLODNode(QTreeWidgetItem* pTopLevel, size_t lod);

protected:
	vector<QTreeMaterialViewItem*> vecMaterialViewsItems;

public:
	QByteArray m_qtByteArray;
	ImporterLib::RigidModel::IRigidModelFile* m_poFile = nullptr;
	shared_ptr<RigidModelV2::Common::CommonFile> m_poRMV2FileCommon = nullptr;
};
