#pragma once

#include <vector>
#include <string>
#include <editor/tools/transform/SelectTool.h>
#include <editor/tools/transform/MoveTool.h>
#include <editor/tools/transform/ScaleTool.h>
#include <editor/tools/transform/RotateTool.h>
#include <editor/tools/transform/SelectTool.h>
#include <editor/tools/PlayTool.h>
#include <QToolBar>

namespace ToolManager {
    void createTools(QToolBar* parent);
    QToolButton* createQuickTool(const QString& objectName, QWidget* parent = nullptr);
}