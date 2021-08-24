#pragma once

#include <memory>
#include <QtCore/qmimedata.h>
#include <QtCore/qdir.h>

#include <QtWidgets/qwidget.h>

#include <QtWidgets/qwidget.h>
#include <QtWidgets/qlineedit.h>
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
#include <qevent.h>

using namespace std;

class QRMV2Widget : public QWidget, public Ui::QRMV2Widget
{
	Q_OBJECT

public:
	QRMV2Widget(QWidget* parent = Q_NULLPTR);
	~QRMV2Widget();

	void fixRMV2LodValues();

#ifdef _DEBUG

	void dragEnterEvent(QDragEnterEvent* event)
	{
		// if some actions should not be usable, like move, this code must be adopted
		event->acceptProposedAction();
	}

	void dragMoveEvent(QDragMoveEvent* event)
	{
		// if some actions should not be usable, like move, this code must be adopted
		event->acceptProposedAction();
	}

	void dragLeaveEvent(QDragLeaveEvent* event)
	{
		event->accept();
	}

	void dropEvent(QDropEvent* event) override
	{
		auto poMimeData = event->mimeData();

		auto text = poMimeData->text();
		auto urls = poMimeData->urls();

		wstring wstrFilePath = urls[0].toLocalFile().toStdWString();

		auto spoImporter = RigidModelV2::File_Importer_Common::create(wstrFilePath);

		event->accept();

		if (!spoImporter)
		{
			return;
		}

		if (spoImporter->getFile())
		{
			setData_New(spoImporter->getFile());
			return;
		}

		return;

		int debug_1 = 1;
	}

#endif // DEBUG

	bool setRigidModel(ImporterLib::RigidModel::IRigidModelFile*);
	bool setData_New(shared_ptr<RigidModelV2::Common::CommonFile> _spoCommonFile);

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
	//ImporterLib::RigidModel::IRigidModelFile* m_poFile = nullptr;
	shared_ptr<RigidModelV2::Common::CommonFile> m_poRMV2FileCommon = nullptr;
};
