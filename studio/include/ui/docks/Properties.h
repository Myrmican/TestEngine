#pragma once

#include <QDockWidget>
#include <QObject>
#include <QEvent>
#include <project/Project.h>

class MainWindow;

class QTreeWidgetItem;
namespace Engine {
    class Instance;
    class Property;
}

class Properties : public QDockWidget {
    Q_OBJECT

public:
    Project* m_project;
    QTreeWidget* treeWidget;

    Properties(MainWindow* window, Project* project);
    void AddProperty(Engine::Instance* instance, const Engine::Property* property);
    QTreeWidgetItem* GetOrCreateCategory(const std::string& categoryName);
    void InspectInstance(Engine::Instance* selectedInstance);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
};