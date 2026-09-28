#include <core/InstanceHandler.h>
#include <engine/core/Reflection.h>
#include <ui/docks/Explorer.h>
#include <datamodel/Instance.h>
#include <QMainWindow>
#include <QTreeWidgetItem>
#include <string>

namespace Engine {
    InsertInstanceSet insertInstance(std::string className, QTreeWidgetItem* parentItem, QMainWindow* mainWindow) {
        if (!mainWindow) return {};

        Explorer* explorer = mainWindow->findChild<Explorer*>();
        if (!explorer) return {};

        std::unique_ptr<Engine::Creatable> newInstance = Engine::CreateInstance(className);
        Instance* parentInstance = Engine::GetEngineInstance(parentItem);

        if (!parentInstance && explorer->m_project->engine->getDataModel())
            parentInstance = explorer->m_project->engine->getDataModel();

        if (!parentInstance) {
            return {};
        }

        Instance* rawInstance = newInstance.get();
        parentInstance->addChild(std::move(newInstance));

        QTreeWidgetItem* treeItem = explorer->AddItem(parentItem, rawInstance);
        return InsertInstanceSet{ treeItem, rawInstance };
    }
}