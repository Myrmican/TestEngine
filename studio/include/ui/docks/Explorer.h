#pragma once

#include <QDockWidget>
#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QObject>
#include <QEvent>
#include <project/Project.h>
#include <datamodel/Game.h>

class Explorer : public QObject {
    Q_OBJECT

public:
    Project* m_project;

    QDockWidget* dockWidget;
	QTreeWidget* treeWidget;

    Explorer(QMainWindow* window, Project* project);

    QTreeWidgetItem* AddItem(QTreeWidgetItem* parentItem, Engine::Instance* instance);

    void AssembleRoot();

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
};