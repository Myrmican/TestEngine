#pragma once

#include <QDockWidget>
#include <QTreeWidgetItem>
#include <QObject>
#include <QEvent>
#include <project/Project.h>
#include <datamodel/DataModel.h>

class MainWindow;

namespace Engine {
    QTreeWidgetItem* GetEngineInstance(QTreeWidget* treeWidget, Instance* instance);
	Instance* GetEngineInstance(QTreeWidgetItem* item);
}

class Explorer : public QDockWidget {
    Q_OBJECT

public:
    Project* m_project;
	QTreeWidget* treeWidget;

    Explorer(MainWindow* window, Project* project);

    QTreeWidgetItem* AddItem(QTreeWidgetItem* parentItem, Engine::Instance* instance);

    void AssembleRoot();

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
};