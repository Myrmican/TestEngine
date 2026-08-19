#pragma once

#include <QDockWidget>
#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QObject>
#include <QEvent>
#include <project/Project.h>

class Explorer : public QObject {
    Q_OBJECT

public:
    QDockWidget* dockWidget;
	QTreeWidget* treeWidget;

    Explorer(QMainWindow* window, Project* project);

    void AddItem(QTreeWidgetItem* parentItem, const QString& itemName, const std::string className);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
};