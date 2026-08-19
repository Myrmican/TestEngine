#include <ui/menus/PluginsMenu.h>

PluginsMenu::PluginsMenu(QMenuBar* menuBar) {
    QMenu* pluginsMenu = menuBar->addMenu("Plugins");
    pluginsMenu->addAction("Manage plugins");
    pluginsMenu->addAction("Plugin folder");
}