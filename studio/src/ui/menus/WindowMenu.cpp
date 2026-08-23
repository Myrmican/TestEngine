#include <ui/menus/WindowMenu.h>
#include <ui/menus/MenuManager.h>

WindowMenu::WindowMenu(QMenuBar* menuBar) {
    QMenu* windowMenu = Menu::create(menuBar, "Window");
    menuBar->addMenu(windowMenu);

    QMenu* windowScriptMenu = windowMenu->addMenu("Script");
    QAction* scriptFind = windowScriptMenu->addAction("Find In Place");
}