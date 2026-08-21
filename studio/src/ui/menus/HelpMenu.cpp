#include <ui/menus/HelpMenu.h>
#include <ui/MenuManager.h>
#include <QDesktopServices>
#include <QUrl>

HelpMenu::HelpMenu(QMenuBar* menuBar) {
    QMenu* helpMenu = Menu::create(menuBar, "Help");
    helpMenu->addAction("Documentation");
    helpMenu->addAction("Forum", []() { QDesktopServices::openUrl(QUrl("https://devforum.roblox.com")); });
}