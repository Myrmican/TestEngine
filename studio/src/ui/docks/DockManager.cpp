#include <QMainWindow>
#include <project/Project.h>
#include <ui/docks/Explorer.h>
#include <ui/docks/Properties.h>
#include <ui/docks/Output.h>
#include <ui/docks/Toolbox.h>
#include <ui/docks/DockManager.h>

namespace DockManager {

	DockSet setup(QMainWindow* window, Project* project) {
        Explorer* explorerDock = new Explorer(window, project);
        Properties* propertiesDock = new Properties(window, project);
        Output* outputDock = new Output(window, project);
        Toolbox* toolboxDock = new Toolbox(window, project);

        toolboxDock->dockWidget->setVisible(false);

        window->setDockNestingEnabled(true);

        window->addDockWidget(Qt::RightDockWidgetArea, explorerDock->dockWidget);
        window->addDockWidget(Qt::LeftDockWidgetArea, propertiesDock->dockWidget);
        window->addDockWidget(Qt::BottomDockWidgetArea, outputDock->dockWidget);
        window->addDockWidget(Qt::LeftDockWidgetArea, toolboxDock->dockWidget);

        return { explorerDock, propertiesDock, outputDock, toolboxDock };
	}
}