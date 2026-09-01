#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QListWidget>

class QTreeWidgetItem;

namespace Engine {
	class InsertObjectPopup : public QWidget {
		Q_OBJECT
	public:
		InsertObjectPopup(QWidget* parent = nullptr, QTreeWidgetItem* item = nullptr);

	private:
		QLineEdit* searchBar;
		QListWidget* objectList;
	};
}