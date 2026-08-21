#include <ui/menus/TestMenu.h>
#include <ui/MenuManager.h>

TestMenu::TestMenu(QMenuBar* menuBar) {
    QMenu* testMenu = Menu::create(menuBar, "Test");
    QMenu* startTestMenu = Menu::create(testMenu, "Start Test");

	startTestMenu->addAction("Play");

    testMenu->addSeparator();
    testMenu->addAction("Device Emulator");
    testMenu->addAction("Add Client");
    testMenu->addSeparator();
    testMenu->addAction("End Session");
}