#include <ui/menus/WindowMenu.h>
#include <ui/menus/MenuManager.h>

WindowMenu::WindowMenu(QMenuBar* menuBar) {
    QMenu* windowMenu = Menu::create(menuBar, "Window");
    menuBar->addMenu(windowMenu);

    QMenu* scriptMenu = Menu::create(menuBar, "Script");

    QAction* windowScriptMenu = windowMenu->addMenu(scriptMenu);
    QAction* scriptFind = scriptMenu->addAction("Find In Place");
}