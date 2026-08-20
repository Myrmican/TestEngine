#include <ui/menus/EditMenu.h>
#include <ui/MenuManager.h>

EditMenu::EditMenu(QMenuBar* menuBar) {
    QMenu* editMenu = Menu::create(menuBar, "Edit");
    for (const QString& action : { "Undo", "Redo", "-", "Cut", "Copy", "Paste", "Paste Into", "Duplicate", "Delete" }) {
        if (action == "-") editMenu->addSeparator();
        else editMenu->addAction(action);
    }
}