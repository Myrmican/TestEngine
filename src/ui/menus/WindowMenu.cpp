#include <ui/menus/WindowMenu.h>

WindowMenu::WindowMenu(QMenuBar* menuBar) {
    QMenu* windowMenu = menuBar->addMenu("Window");
    

    QMenu* windowScriptMenu = windowMenu->addMenu("Script");
    QAction* scriptFind = windowScriptMenu->addAction("Find In Place");
}