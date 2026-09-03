#include <ui/menus/ViewMenu.h>
#include <ui/docks/Explorer.h>
#include <ui/menus/MenuManager.h>
#include <project/Project.h>
#include <QDockWidget>
#include <iostream>

void ToggleVisibility(std::string dockName, QMainWindow* window) {
    QDockWidget* dockWindow = window->findChild<QDockWidget*>(dockName);

    bool isVisible = dockWindow->isVisible();

    if (!isVisible) {
        dockWindow->setVisible(true);
        return;
    }

    dockWindow->setVisible(false);
}

ViewMenu::ViewMenu(QMenuBar* menuBar, QMainWindow* window) {
    QMenu* viewMenu = Menu::create(menuBar, "View");
    menuBar->addMenu(viewMenu);

    QAction* toolboxAction = viewMenu->addAction("Toolbox", [window] {
        ToggleVisibility("ToolboxDock", window);
        });
    QAction* explorerAction = viewMenu->addAction("Explorer", [window] {
        ToggleVisibility("ExplorerDock", window);
        });
    QAction* propertiesAction = viewMenu->addAction("Properties", [window] {
        ToggleVisibility("PropertiesDock", window);
        });
    QAction* outputAction = viewMenu->addAction("Output", [window] {
        ToggleVisibility("OutputDock", window);
        });

    toolboxAction->setCheckable(true);
    explorerAction->setCheckable(true);
    explorerAction->setChecked(true);
    propertiesAction->setCheckable(true);
    propertiesAction->setChecked(true);
    outputAction->setCheckable(true);
    outputAction->setChecked(true);
}