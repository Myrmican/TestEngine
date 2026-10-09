#pragma once

#include <vector>
#include <string>
#include <editor/tools/PlayTool.h>
#include <QToolBar>

namespace ToolManager {
    QWidget* createTools(QToolBar* parent, std::string tabName);
    QToolButton* createQuickTool(const QString& objectName, QWidget* parent = nullptr);
}