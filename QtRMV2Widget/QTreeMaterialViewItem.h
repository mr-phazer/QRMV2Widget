#pragma once

#include <memory>

#include <QWidget>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qtextedit.h>
#include <QtWidgets/qFrame.h>
#include <qobject.h>

#include "ui_QRMV2Widget.h"
#include "ui_QRMV2Widget.h"

#include "..\ImporterLib\RigidModelFileStructs.h"

#include "QMaterialEditView.h"

namespace TreeItemType
{
	const int MaterialEditView = QTreeWidgetItem::UserType + 1;
}

using namespace std;

class QTreeItemText : public QObject, public QTreeWidgetItem
{
	Q_OBJECT

public:
	QTreeItemText(const QStringList& strings, int type = Type)
		: QTreeWidgetItem(strings, TreeItemType::MaterialEditView)
	{}

	QTreeItemText(QTreeWidget* pTreeWidget, QStringList& strings, int type = Type)
		: QTreeWidgetItem(strings, TreeItemType::MaterialEditView)
	{
		//connect(pTreeWidget, &QTreeWidget::itemClicked, this, &QRMV2Widget::onItemClicked);

		qstrContent = strings[0];
	}

	QString qstrContent = QString("test");
	QTextEdit* pqTextEdit = nullptr;

	QTextEdit* m_pTextEdit = nullptr;;
};

class QTreeMaterialViewItem : public QObject, public QTreeWidgetItem
{
	Q_OBJECT

public:
	QTreeMaterialViewItem(const QStringList& strings, int type = Type)
		: QTreeWidgetItem(strings, TreeItemType::MaterialEditView)

	{
		int debug_a = 1;
	}

	virtual ~QTreeMaterialViewItem()
	{
		int debug_a = 1;
	}

	QTreeMaterialViewItem(QTreeWidget* pTreeWidget, QStringList& strings, int type = Type)
		: QTreeWidgetItem(strings, TreeItemType::MaterialEditView)
	{
		//connect(pTreeWidget, &QTreeWidget::itemClicked, this, &QRMV2Widget::onItemClicked);
	}

	QMaterialEditView* m_pMaterialView = nullptr;
};