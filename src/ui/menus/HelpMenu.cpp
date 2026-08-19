#include <ui/menus/HelpMenu.h>
#include <QDesktopServices>
#include <QUrl>

HelpMenu::HelpMenu(QMenuBar* menuBar) {
    QMenu* helpMenu = menuBar->addMenu("Help");
    helpMenu->addAction("Documentation");
    helpMenu->addAction("Forum", []() { QDesktopServices::openUrl(QUrl("https://devforum.roblox.com")); });
}