#include <ui/menus/HelpMenu.h>
#include <ui/menus/MenuManager.h>
#include <QDesktopServices>
#include <QUrl>

HelpMenu::HelpMenu(QMenuBar* menuBar) {
    QMenu* helpMenu = Menu::create(menuBar, "Help");
    menuBar->addMenu(helpMenu);
    helpMenu->addAction("Documentation");
    helpMenu->addAction("Forum", []() { QDesktopServices::openUrl(QUrl("https://devforum.roblox.com")); });
}