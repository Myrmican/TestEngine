#include <ui/menus/TestMenu.h>

TestMenu::TestMenu(QMenuBar* menuBar) {
    QMenu* testMenu = menuBar->addMenu("Test");
    QMenu* startTestMenu = testMenu->addMenu("Start Test");

	startTestMenu->addAction("Play");

    testMenu->addSeparator();
    testMenu->addAction("Device Emulator");
    testMenu->addAction("Add Client");
    testMenu->addSeparator();
    testMenu->addAction("End Session");
}