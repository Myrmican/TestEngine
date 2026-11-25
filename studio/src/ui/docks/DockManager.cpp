#include <QMainWindow>
#include <project/Project.h>
#include <ui/docks/Explorer.h>
#include <ui/docks/Properties.h>
#include <ui/docks/Output.h>
#include <ui/docks/DockManager.h>
#include <ui/docks/Assets.h>

namespace EngineEditor {

    DockManager::DockManager(QMainWindow* window, Project* project) {
        explorerDock = new Explorer(window, project);
        propertiesDock = new Properties(window, project);
        outputDock = new Output(window, project);
        assetsDock = new Assets(window, project);

        assetsDock->setVisible(false);

        window->setDockNestingEnabled(true);

        window->addDockWidget(Qt::RightDockWidgetArea, explorerDock);
        window->addDockWidget(Qt::LeftDockWidgetArea, propertiesDock);
        window->addDockWidget(Qt::BottomDockWidgetArea, outputDock);
        window->addDockWidget(Qt::RightDockWidgetArea, assetsDock);

        window->splitDockWidget(outputDock, assetsDock, Qt::Orientation::Horizontal);
    }
}