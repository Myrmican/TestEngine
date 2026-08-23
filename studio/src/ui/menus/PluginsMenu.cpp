#include <ui/menus/PluginsMenu.h>
#include <ui/menus/MenuManager.h>

PluginsMenu::PluginsMenu(QMenuBar* menuBar) {
    QMenu* pluginsMenu = Menu::create(menuBar, "Plugins");
    menuBar->addMenu(pluginsMenu);
    pluginsMenu->addAction("Manage plugins");
    pluginsMenu->addAction("Plugin folder");
}