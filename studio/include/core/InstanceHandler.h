#pragma once

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <string>
#include <engine/datamodel/Instance.h>

namespace Engine {
    struct InsertInstanceSet {
        QTreeWidgetItem* item;
        Instance* instance;
    };

    InsertInstanceSet insertInstance(std::string className, QTreeWidgetItem* parentItem, QMainWindow* mainWindow);
}