#include <ui/menus/PluginsMenu.h>
#include <ui/MenuManager.h>

PluginsMenu::PluginsMenu(QMenuBar* menuBar) {
    QMenu* pluginsMenu = Menu::create(menuBar, "Plugins");
    pluginsMenu->addAction("Manage plugins");
    pluginsMenu->addAction("Plugin folder");
}