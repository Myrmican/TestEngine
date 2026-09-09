#pragma once

#include <QDockWidget>
#include <QMainWindow>
#include <QObject>
#include <QEvent>
#include <project/Project.h>

class QTreeWidgetItem;
namespace Engine {
    class Instance;
    class Property;
}

class Properties : public QObject {
    Q_OBJECT

public:
    Project* m_project;
    QDockWidget* dockWidget;
    QTreeWidget* treeWidget;

    Properties(QMainWindow* window, Project* project);
    void AddProperty(Engine::Instance* instance, const Engine::Property* property);
    QTreeWidgetItem* GetOrCreateCategory(const std::string& categoryName);
    void InspectInstance(Engine::Instance* selectedInstance);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
};