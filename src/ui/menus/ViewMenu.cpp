#include <ui/menus/ViewMenu.h>

ViewMenu::ViewMenu(QMenuBar* menuBar) {
    QMenu* viewMenu = menuBar->addMenu("View");
    viewMenu->addAction("Toolbox");
    viewMenu->addAction("Explorer");
    viewMenu->addAction("Properties");
    viewMenu->addAction("Output");
}