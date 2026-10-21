#pragma once

#include <vector>
#include <string>
#include <editor/tools/PlayTool.h>
#include <QStackedWidget>

namespace ToolManager {
    QWidget* createTools(QStackedWidget* parent, std::string tabName);
    QToolButton* createQuickTool(const QString& objectName, QWidget* parent = nullptr);
}