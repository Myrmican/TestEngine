#pragma once

#include <QTreeWidgetItem>
#include <string>
#include <engine/datamodel/Instance.h>

class MainWindow;

namespace Engine {
    struct InsertInstanceSet {
        QTreeWidgetItem* item;
        Instance* instance;
    };

    InsertInstanceSet insertInstance(std::string className, QTreeWidgetItem* parentItem, MainWindow* mainWindow);
}