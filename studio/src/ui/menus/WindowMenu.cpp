#include <ui/menus/WindowMenu.h>
#include <ui/MenuManager.h>

WindowMenu::WindowMenu(QMenuBar* menuBar) {
    QMenu* windowMenu = Menu::create(menuBar, "Window");
    

    QMenu* windowScriptMenu = windowMenu->addMenu("Script");
    QAction* scriptFind = windowScriptMenu->addAction("Find In Place");
}