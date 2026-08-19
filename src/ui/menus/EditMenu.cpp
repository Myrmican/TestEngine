#include <ui/menus/EditMenu.h>

EditMenu::EditMenu(QMenuBar* menuBar) {
    QMenu* editMenu = menuBar->addMenu("Edit");
    for (const QString& action : { "Undo", "Redo", "-", "Cut", "Copy", "Paste", "Paste Into", "Duplicate", "Delete" }) {
        if (action == "-") editMenu->addSeparator();
        else editMenu->addAction(action);
    }
}