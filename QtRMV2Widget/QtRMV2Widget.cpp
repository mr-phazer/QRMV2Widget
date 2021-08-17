#include "QtRMV2Widget.h"
#include "qfiledialog.h"
#include "..\ImporterLib\RigidModelV2_Reader.h"
#include "..\ImporterLib\RigidModelV2_Writer.h"
#include "..\SystemLib\tools.h"

void QRMV2Widget::fixRMV2LodValues()
{
	QFileDialog* qDialog = new QFileDialog(this);

	qDialog->setNameFilters({ "RMV2 (*.rigid_model_v2)" ,  "All Files (*.*.)" });
	qDialog->setFileMode(QFileDialog::ExistingFiles);

	qDialog->exec();
	auto filesSelected = qDialog->selectedFiles();

	for (auto& it : filesSelected)
	{
		auto importer = RigidModelV2::File_Importer_Common::create(it.toStdWString());
		auto inFile = importer->getFile();

		// --- TODO: process

		if (inFile->oLodHeaderBlock.vecElements.size() == 1)
		{
			inFile->oLodHeaderBlock.vecElements[0].fVisibilityDistance = 10000.f;
		}
		if (inFile->oLodHeaderBlock.vecElements.size() == 2)
		{
			inFile->oLodHeaderBlock.vecElements[0].fVisibilityDistance = 20.f;
			inFile->oLodHeaderBlock.vecElements[1].fVisibilityDistance = 10000.f;
		}
		if (inFile->oLodHeaderBlock.vecElements.size() == 3)
		{
			inFile->oLodHeaderBlock.vecElements[0].fVisibilityDistance = 20.f;
			inFile->oLodHeaderBlock.vecElements[1].fVisibilityDistance = 80.f;
			inFile->oLodHeaderBlock.vecElements[2].fVisibilityDistance = 10000.f;
		}
		if (inFile->oLodHeaderBlock.vecElements.size() == 4)
		{
			inFile->oLodHeaderBlock.vecElements[0].fVisibilityDistance = 20.f;
			inFile->oLodHeaderBlock.vecElements[1].fVisibilityDistance = 80.f;
			inFile->oLodHeaderBlock.vecElements[2].fVisibilityDistance = 100.f;
			inFile->oLodHeaderBlock.vecElements[3].fVisibilityDistance = 10000.f;
		}
		else if (inFile->oLodHeaderBlock.vecElements.size() == 5)
		{
			inFile->oLodHeaderBlock.vecElements[0].fVisibilityDistance = 20.f;
			inFile->oLodHeaderBlock.vecElements[1].fVisibilityDistance = 80.f;
			inFile->oLodHeaderBlock.vecElements[2].fVisibilityDistance = 100.f;
			inFile->oLodHeaderBlock.vecElements[3].fVisibilityDistance = 300.f;
			inFile->oLodHeaderBlock.vecElements[4].fVisibilityDistance = 10000.f;
		}
		else if (inFile->oLodHeaderBlock.vecElements.size() == 6)
		{
			inFile->oLodHeaderBlock.vecElements[0].fVisibilityDistance = 30.f;
			inFile->oLodHeaderBlock.vecElements[1].fVisibilityDistance = 60.f;
			inFile->oLodHeaderBlock.vecElements[2].fVisibilityDistance = 120.f;
			inFile->oLodHeaderBlock.vecElements[3].fVisibilityDistance = 250.f;
			inFile->oLodHeaderBlock.vecElements[4].fVisibilityDistance = 350.f;
			inFile->oLodHeaderBlock.vecElements[5].fVisibilityDistance = 10000.f;
		}

		//inFile->oLodHeaderBlock.vecElements[1].fVisibilityDistance = 20.f;

		auto exporter = RigidModelV2::File_Exporter_Common::create_empty();
		exporter->setFileData(inFile);

		std::wstring strDir = tools::GetFileFromFullPath(it.toStdWString());
		strDir = LR"(c:\temp\RMV2\)" + strDir;
		exporter->write_file(strDir);

		auto debug_2 = 1;
	}

	int debug_1 = 1;
}

QRMV2Widget::QRMV2Widget(QWidget* parent)
	: QWidget(parent)
{
	setAcceptDrops(true);
	setupUi(this);

	checkBox->setVisible(false);

	connect(pushButton_CopyTextureGroup, &QPushButton::clicked, this, &QRMV2Widget::onGroupCopy_Texture_ButtonClicked);
	connect(pushButton_CopyAlphaAlphaGroup, &QPushButton::clicked, this, &QRMV2Widget::onGroupCopy_Alpha_ButtonClicked);
}

QRMV2Widget::~QRMV2Widget()
{
	int debug_a = 1;
}

bool QRMV2Widget::setRigidModel(ImporterLib::RigidModel::IRigidModelFile* _poFileIput)
{
#if 0
	treeWidget->clear();
	m_poFile = _poFileIput;

	if (m_pPStackedWidget)
	{
		m_pPStackedWidget->deleteLater();
	}
	m_pPStackedWidget = new QStackedWidget();
	verticalLayout_RightView->insertWidget(0, m_pPStackedWidget);

	//treeWidget->headerItem()->setText(0, _poFile->strFileName.c_str());

	QTreeWidgetItem* pTopLevel = treeWidget->invisibleRootItem();
	/*QTreeWidgetItem* pTopLevel = new QTreeWidgetItem({ "RMV2 Structure" });
	treeWidget->addTopLevelItem(pTopLevel);*/

	insertFileHeader(m_poFile, pTopLevel);
	insertLODHeaders(m_poFile, pTopLevel);

	auto& _pITem = pTopLevel;

	QTreeWidgetItem* pItem = nullptr;

	// set header to file title
	//treeWidget->setHeaderLabel(_poFile->strFileName.c_str());
	treeWidget->setHeaderLabel("File Structure:");

	// iterate through LODs
	for (size_t lod = 0; lod < m_poFile->oFileHeader.Count.wLod; lod++)
	{
		auto pLodItem = insertLODNode(pTopLevel, lod);

		bool bItemSet = false;
		// iterate through groups
		for (size_t group = 0; group < m_poFile->vecLodData[lod].vecGroups.size(); group++)
		{
			auto pGroupItem = createMaterialViewTreeItem(pLodItem, string(m_poFile->vecLodData[lod].vecGroups[group].oHeader.szGroupName));

			// populate the material widget
			pGroupItem->m_pMaterialView = new QMaterialEditView();
			size_t t = 0;
			for (; t < m_poFile->vecLodData[lod].vecGroups[group].oHeader.dwTextureCount; t++)
			{
				// create "texture edit line widget, bind the correct data
				pGroupItem->m_pMaterialView->addTextureLine(t,
					&m_poFile->vecLodData[lod].vecGroups[group].oMaterial.vecTextures[t].szTextureDirAndFileName,
					&m_poFile->vecLodData[lod].vecGroups[group].oMaterial.vecTextures[t].Type);
			}

			// create "alpha edit line", bind it to the data it is to edit
			auto pAlphaEditLine = pGroupItem->m_pMaterialView->addAlphaAditLine(
				pGroupItem->m_pMaterialView,
				&m_poFile->vecLodData[lod].vecGroups[group].oMaterial.alpha,
				&m_poFile->vecLodData[lod].vecGroups[group].oMaterial.mask
			);
			// and add it to the "material view" layout
			pGroupItem->m_pMaterialView->verticalLayout->insertWidget(t++, pAlphaEditLine);

			// create group tree node and set propertiesonItemClicked
			pGroupItem->m_pMaterialView->addGroupInfoWidget(t++,
				&m_poFile->vecLodData[lod].vecGroups[group].oHeader.uiVertextCount,
				&m_poFile->vecLodData[lod].vecGroups[group].oHeader.uiIndexCount,
				&m_poFile->vecLodData[lod].vecGroups[group].oHeader.RigidMaterialId,
				&m_poFile->vecLodData[lod].vecGroups[group].oHeader.VertexFormatId
			);

			// add view to "widget stack"
			m_pPStackedWidget->addWidget(pGroupItem->m_pMaterialView);

			// handle event "click on tree node" to show the right panel
			connect(treeWidget, &QTreeWidget::itemClicked, this, &QRMV2Widget::onItemClicked);

			// handle "selection changed"
			connect(treeWidget, &QTreeWidget::itemSelectionChanged, this, &QRMV2Widget::onItemSelectionChange);

			if (!bItemSet)
			{
				treeWidget->setCurrentItem(pGroupItem);
				pItem = pGroupItem;
				bItemSet = true;
			}
}
}

	QWidget* pQ = new QWidget;
	m_pPStackedWidget->addWidget(pQ);
	m_pPStackedWidget->setCurrentWidget(pQ);

	emit treeWidget->itemClicked(pItem, 0);

	//onItemClicked(pItem, 0);
#endif
	return true;
}

bool QRMV2Widget::setData_New(shared_ptr<RigidModelV2::Common::CommonFile> _spoCommonFile)
{
	if (!_spoCommonFile)
	{
		return false;
	}

	m_poRMV2FileCommon = _spoCommonFile;

	treeWidget->clear();

	if (m_pPStackedWidget)
	{
		m_pPStackedWidget->deleteLater();
	}

	m_pPStackedWidget = new QStackedWidget();
	verticalLayout_TextureList->insertWidget(0, m_pPStackedWidget);

	//treeWidget->headerItem()->setText(0, _poFile->strFileName.c_str());

	QTreeWidgetItem* pTopLevel = treeWidget->invisibleRootItem();
	/*QTreeWidgetItem* pTopLevel = new QTreeWidgetItem({ "RMV2 Structure" });
	treeWidget->addTopLevelItem(pTopLevel);*/
	QLayoutItem* child;
	while ((child = verticalLayout_ErrorMsg->takeAt(0)) != nullptr) {
		delete child->widget(); // delete the widget
		delete child;   // delete the layout item
	}
	if (m_poRMV2FileCommon->getLastErrorString() != "")
	{
		auto poLabel = new QLabel(m_poRMV2FileCommon->getLastErrorString().c_str(), this);

		poLabel->setStyleSheet("color: white;"
			"background-color: red;"
			"font-size: 16px;"
			"border-style: solid;"
			"border-width: 2px;"
			"border-radius: 10px;"
			"border-style: solid;"
			"border-color: white;"
			"padding"
		);

		verticalLayout_ErrorMsg->insertWidget(0, poLabel);
		return false;
	}

	insertFileHeader_New(pTopLevel);
	insertLODHeaders_New(pTopLevel);

	auto& _pITem = pTopLevel;

	QTreeWidgetItem* pItem = nullptr;

	// set header to file title
	//treeWidget->setHeaderLabel(_poFile->strFileName.c_str());
	treeWidget->setHeaderLabel("File Structure:");

	// iterate through LODs
	for (size_t lod = 0; lod < m_poRMV2FileCommon->vecLODs.size(); lod++)
	{
		auto pLodItem = insertLODNode(pTopLevel, lod);

		bool bItemSet = false;
		// iterate through groups
		for (size_t group = 0; group < m_poRMV2FileCommon->vecLODs[lod].vecMeshBlocks.size(); group++)
		{
			auto pGroupItem = createMaterialViewTreeItem(pLodItem, string(m_poRMV2FileCommon->vecLODs[lod].vecMeshBlocks[group].oSubMeshHeader.strMeshName));

			// populate the material widget
			int layout_offset = 0;
			pGroupItem->m_pMaterialView = new QMaterialEditView();
			size_t t = 0;
			for (; t < m_poRMV2FileCommon->vecLODs[lod].vecMeshBlocks[group].oMaterialBlock.vecTextures.size(); t++)
			{
				// create "texture edit line widget, bind the correct data
				layout_offset = t;
				pGroupItem->m_pMaterialView->addTextureLine(layout_offset,
					&m_poRMV2FileCommon->vecLODs[lod].vecMeshBlocks[group].oMaterialBlock.vecTextures[t].szTextureDirAndFileName,
					&m_poRMV2FileCommon->vecLODs[lod].vecMeshBlocks[group].oMaterialBlock.vecTextures[t].Type);
			}

			// create "alpha edit line", bind it to the data it is to edit
			auto pAlphaEditLine = pGroupItem->m_pMaterialView->addAlphaAditLine(
				pGroupItem->m_pMaterialView,
				&m_poRMV2FileCommon->vecLODs[lod].vecMeshBlocks[group].oMaterialBlock.dwAlphaFlags,
				&m_poRMV2FileCommon->vecLODs[lod].vecMeshBlocks[group].oMaterialBlock.dwMaskFlags,
				&m_poRMV2FileCommon->vecLODs[lod].vecMeshBlocks[group].oSubMeshHeader.strShaderName,
				&m_poRMV2FileCommon->vecLODs[lod].vecMeshBlocks[group].oSubMeshHeader.strMeshName,
				&m_poRMV2FileCommon->vecLODs[lod].vecMeshBlocks[group].oSubMeshHeader.strTextureDirectory
			);
			// and add it to the "material view" layout
			pGroupItem->m_pMaterialView->verticalLayout_Alpha->insertWidget(t++, pAlphaEditLine);

			// create group tree node and set propertiesonItemClicked
			pGroupItem->m_pMaterialView->addGroupInfoWidget(t++,
				&m_poRMV2FileCommon->vecLODs[lod].vecMeshBlocks[group].oPreHeader.dwVertexCount,
				&m_poRMV2FileCommon->vecLODs[lod].vecMeshBlocks[group].oPreHeader.dwIndexCount,
				&m_poRMV2FileCommon->vecLODs[lod].vecMeshBlocks[group].oPreHeader.RigidMaterialId,
				&m_poRMV2FileCommon->vecLODs[lod].vecMeshBlocks[group].oSubMeshHeader.VertexFormatId
			);

			// add view to "widget stack"
			m_pPStackedWidget->addWidget(pGroupItem->m_pMaterialView);

			// handle event "click on tree node" to show the right panel
			connect(treeWidget, &QTreeWidget::itemClicked, this, &QRMV2Widget::onItemClicked);

			// handle "selection changed"
			connect(treeWidget, &QTreeWidget::itemSelectionChanged, this, &QRMV2Widget::onItemSelectionChange);

			if (!bItemSet)
			{
				treeWidget->setCurrentItem(pGroupItem);
				pItem = pGroupItem;
				bItemSet = true;
			}
		}
	}

	QWidget* pQ = new QWidget;
	m_pPStackedWidget->addWidget(pQ);
	m_pPStackedWidget->setCurrentWidget(pQ);

	emit treeWidget->itemClicked(pItem, 0);

	//onItemClicked(pItem, 0);

	return true;
}

void QRMV2Widget::onItemClicked(QTreeWidgetItem* item, int column)
{
	QTreeMaterialViewItem* p = static_cast<QTreeMaterialViewItem*>(item);

	if (!p)
	{
		return;
	}

	m_pPStackedWidget->setCurrentWidget(p->m_pMaterialView);
}

void QRMV2Widget::onItemSelectionChange()
{
	auto vecItem = treeWidget->selectedItems();
	if (vecItem.size() == 0)
	{
		return;
	}

	QTreeMaterialViewItem* p = static_cast<QTreeMaterialViewItem*>(vecItem[0]);

	if (!p)
	{
		return;
	}

	m_pPStackedWidget->setCurrentWidget(p->m_pMaterialView);
}

void QRMV2Widget::onGroupCopy_Texture_ButtonClicked()
{
	QTreeWidgetItem* pSelectedItem = treeWidget->currentItem();

	if (!pSelectedItem)
	{
		return;
	}

	if (pSelectedItem->type() == TreeItemType::MaterialEditView)
	{
		QTreeMaterialViewItem* pMatViewItem = static_cast<QTreeMaterialViewItem*>(pSelectedItem);

		if (!pMatViewItem)
		{
			return;
		}

		auto textures = pMatViewItem->m_pMaterialView->getTextues();

		int i = 0;

		for (auto& it : vecMaterialViewsItems)
		{
			if (it->type() == TreeItemType::MaterialEditView)
			{
				if (it->checkState(0) == Qt::CheckState::Checked)
				{
					it->m_pMaterialView->setTextures(textures);
				}
			}
		}
	}
}

void QRMV2Widget::onGroupCopy_Alpha_ButtonClicked()
{
	QTreeWidgetItem* pSelectedItem = treeWidget->currentItem();

	if (!pSelectedItem)
	{
		return;
	}

	if (pSelectedItem->type() == TreeItemType::MaterialEditView)
	{
		QTreeMaterialViewItem* pMatViewItem = static_cast<QTreeMaterialViewItem*>(pSelectedItem);

		if (!pMatViewItem)
		{
			return;
		}

		uint32_t alpha_value = pMatViewItem->m_pMaterialView->getAlpha();

		for (auto& it : vecMaterialViewsItems)
		{
			if (it->type() == TreeItemType::MaterialEditView)
			{
				if (it->checkState(0) == Qt::CheckState::Checked)
				{
					it->m_pMaterialView->setAlpha(alpha_value);
				}
			}
		}
	}
}

void QRMV2Widget::insertFileHeader(ImporterLib::RigidModel::IRigidModelFile* _poFile, QTreeWidgetItem* _poParent)
{
	QTreeWidgetItem* pFileHeader = new QTreeWidgetItem({ "FileHeader" });
	_poParent->addChild(pFileHeader);
	pFileHeader->setExpanded(true);
	QFont font;
	font.setBold(true);
	pFileHeader->setFont(0, font);

	// get "magic chars" = "RMV"
	string strSignature = "";
	for (size_t i = 0; i < 4; i++)
	{
		strSignature +=
			_poFile->oFileHeader.Signature.szSignature[i];
	}

	QTreeWidgetItem* pFileSignature = new QTreeWidgetItem(
		{ std::string(R"(Format Signature Chars: ")" + strSignature + R"(")").c_str() }
	);
	pFileHeader->addChild(pFileSignature);

	QTreeWidgetItem* pFileFormatVersion = new QTreeWidgetItem(
		{ ("Format Version: " + to_string(_poFile->oFileHeader.ModelVersion)).c_str() }
	);
	pFileHeader->addChild(pFileFormatVersion);
}

void QRMV2Widget::insertLODHeaders(ImporterLib::RigidModel::IRigidModelFile* _poFIle, QTreeWidgetItem* _poParent)
{
	QTreeWidgetItem* pLODHeaders = new QTreeWidgetItem({ "LOD Headers" });
	_poParent->addChild(pLODHeaders);
	//pFileHeader->setExpanded(true);
	QFont font;
	font.setBold(true);
	pLODHeaders->setFont(0, font);

	for (size_t i = 0; i < _poFIle->spovecLODHeaders.size(); i++)
	{
		QTreeWidgetItem* pLODHeader = new QTreeWidgetItem({ string("LOD Header " + to_string(i)).c_str() });
		pLODHeaders->addChild(pLODHeader);

		QTreeWidgetItem* pGroupCount = new QTreeWidgetItem({ string("Group Count: " + to_string(_poFIle->spovecLODHeaders[i]->dwGroupCount)).c_str() });
		pLODHeader->addChild(pGroupCount);

		QTreeWidgetItem* pZoom = new QTreeWidgetItem({ string("Distance: " + to_string(_poFIle->spovecLODHeaders[i]->fVisibilityDistance)).c_str() });
		pLODHeader->addChild(pZoom);
	}
}

void QRMV2Widget::insertFileHeader_New(QTreeWidgetItem* _poParent)
{
	// ***  FILE HEADER ROOT item ***
	QTreeWidgetItem* pFileHeader = new QTreeWidgetItem({ "FileHeader" });
	_poParent->addChild(pFileHeader);
	pFileHeader->setExpanded(true);
	QFont font;
	font.setBold(true);
	pFileHeader->setFont(0, font);

	// get "magic chars" = "RMV"
	string strSignature = "";
	for (size_t i = 0; i < 4; i++)
	{
		strSignature +=
			m_poRMV2FileCommon->oFileHeader.oFileInfoHeader.Signature.szSignature[i];
	}

	QString qstrGamesUseThisVersion = "";

	switch (m_poRMV2FileCommon->oFileHeader.oFileInfoHeader.dwModelVersion)
	{
	case 5:
	case 6:
		qstrGamesUseThisVersion = "Rome2/Attila/Thrones";
		break;

	case 7:
		qstrGamesUseThisVersion = "WH/WH2/Troy";
		break;

	case 8:
		qstrGamesUseThisVersion = "3K";
		break;

	default:
		qstrGamesUseThisVersion = "unknown";
	}

	// "RMV2" maagic chars
	QTreeWidgetItem* pFileSignatureItem = new QTreeWidgetItem(
		{ std::string(R"(Format Signature: ")" + strSignature + R"(")").c_str() }
	);
	pFileHeader->addChild(pFileSignatureItem);

	// file format version
	QTreeWidgetItem* pFileFormatVersionItem = new QTreeWidgetItem(
		{ "Version: " + QString::number(m_poRMV2FileCommon->oFileHeader.oFileInfoHeader.dwModelVersion) + " (" + qstrGamesUseThisVersion + ")" }
	);
	pFileHeader->addChild(pFileFormatVersionItem);

	// skeleton id string

	/*------------------
	* Skeleton ID item
	--------------------*/
	// make sure the skeleton id string has the proper 0 termination (maybe/propabaly not needed)
	string strSkeleton;
	strSkeleton.resize(130);
	memcpy((void*)strSkeleton.data(), m_poRMV2FileCommon->oFileHeader.szSkeletonId, 128);

	QTreeWidgetItem* pSkeletonIdItem = new QTreeWidgetItem(
		{ ("Skeleton: " + strSkeleton).c_str() }
	);
	pFileHeader->addChild(pSkeletonIdItem);
	QFont fontSkeleton;
	fontSkeleton.setBold(true);
	fontSkeleton.setFamily("Times");
	pSkeletonIdItem->setFont(0, fontSkeleton);
}

void QRMV2Widget::insertLODHeaders_New(QTreeWidgetItem* _poParent)
{
	QTreeWidgetItem* pLODHeaders = new QTreeWidgetItem({ "LOD Headers" });
	_poParent->addChild(pLODHeaders);
	pLODHeaders->setExpanded(false);
	QFont font;
	font.setBold(true);
	pLODHeaders->setFont(0, font);

	for (size_t i = 0; i < m_poRMV2FileCommon->oLodHeaderBlock.vecElements.size(); i++)
	{
		QTreeWidgetItem* pLODHeaderElementItem = new QTreeWidgetItem({ "[ LOD Header " + QString::number(i + 1) + " ]" });
		pLODHeaders->addChild(pLODHeaderElementItem);

		//QFont fontLodHeaderElement;
		//fontLodHeaderElement.setBold(true);
		//pLODHeaderElementItem->setFont(0, fontLodHeaderElement);

		QTreeWidgetItem* pGroupCountItem = new QTreeWidgetItem({ "Group Count: " + QString::number(m_poRMV2FileCommon->oLodHeaderBlock.vecElements[i].dwMeshCount) });
		pLODHeaderElementItem->addChild(pGroupCountItem);

		QTreeWidgetItem* pZoomItem = new QTreeWidgetItem({ "Visibility Distance: " + QString::number(m_poRMV2FileCommon->oLodHeaderBlock.vecElements[i].fVisibilityDistance) });
		pLODHeaderElementItem->addChild(pZoomItem);

		QTreeWidgetItem* pPolyCountItem = new QTreeWidgetItem({ "Total Polycount: " + QString::number(m_poRMV2FileCommon->oLodHeaderBlock.vecElements[i].dwIndicesDataLength / 6U) });
		pLODHeaderElementItem->addChild(pPolyCountItem);
	}
};

QTreeMaterialViewItem* QRMV2Widget::createMaterialViewTreeItem(QTreeWidgetItem* pLodItem, const std::string& strGroupName)
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

QTreeWidgetItem* QRMV2Widget::insertLODNode(QTreeWidgetItem* pTopLevel, size_t lod)
{
	QTreeWidgetItem* pLodItem = new QTreeWidgetItem(
		{ ("LOD (Level Of Detail)" + to_string(lod + 1)).c_str() }
	);
	pTopLevel->addChild(pLodItem);
	pTopLevel->setExpanded(true);

	QFont font;
	font.setBold(true);
	pLodItem->setFont(0, font);
	pLodItem->setFlags(pLodItem->flags() | Qt::ItemIsUserCheckable);
	pLodItem->setCheckState(0, Qt::CheckState::Unchecked);

	return pLodItem;
}