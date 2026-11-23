#include <QMainWindow>
#include <project/Project.h>
#include <ui/docks/Explorer.h>
#include <ui/docks/Properties.h>
#include <ui/docks/Output.h>
#include <ui/docks/Toolbox.h>
#include <ui/docks/DockManager.h>
#include <ui/docks/Assets.h>

namespace EngineEditor {

    DockManager::DockManager(QMainWindow* window, Project* project) {
        explorerDock = new Explorer(window, project);
        propertiesDock = new Properties(window, project);
        outputDock = new Output(window, project);
        toolboxDock = new Toolbox(window, project);
        assetsDock = new Assets(window, project);

        toolboxDock->setVisible(false);
        assetsDock->setVisible(false);

        window->setDockNestingEnabled(true);

        window->addDockWidget(Qt::RightDockWidgetArea, explorerDock);
        window->addDockWidget(Qt::LeftDockWidgetArea, propertiesDock);
        window->addDockWidget(Qt::BottomDockWidgetArea, outputDock);
        window->addDockWidget(Qt::LeftDockWidgetArea, toolboxDock);
        window->addDockWidget(Qt::RightDockWidgetArea, assetsDock);
    }
}