#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QListWidget>

class QTreeWidgetItem;

namespace Engine {
	class InsertObjectPopup : public QWidget {
		Q_OBJECT
	public:
		InsertObjectPopup(QWidget* widgetParent = nullptr, QTreeWidgetItem* parentItem = nullptr);

	private:
		QLineEdit* searchBar;
		QListWidget* objectList;
	};
}