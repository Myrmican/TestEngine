#include <ui/menus/ViewMenu.h>
#include <ui/docks/Explorer.h>
#include <ui/window/MainWindow.h>
#include <ui/menus/MenuManager.h>
#include <project/Project.h>
#include <QDockWidget>
#include <QAction>
#include <QString>
#include <string>
#include <vector>

std::vector<std::string> dockNames = { "Toolbox", "Explorer", "Properties", "Output", "Assets" };

static bool ToggleVisibility(QString dockName, MainWindow* window) {
    QDockWidget* dockWindow = window->findChild<QDockWidget*>(dockName);
    if (dockWindow == nullptr) return false;

    bool isVisible = dockWindow->isVisible();
    dockWindow->setVisible(!isVisible);

    return isVisible;
}

ViewMenu::ViewMenu(QMenuBar* menuBar, MainWindow* window) {
    QMenu* menu = Menu::create(menuBar, "View");
    menuBar->addMenu(menu);

    for (std::string dock : dockNames) {
        QAction* action = menu->addAction(QString::fromStdString(dock), [window, dock] {
            ToggleVisibility(QString::fromStdString(dock) + "Dock", window);
            });

        action->setObjectName(action->objectName());

        bool isVisible = ToggleVisibility(action->objectName(), window);

        action->setCheckable(true);

        if (isVisible) action->toggle();
    }
}