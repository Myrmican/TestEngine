#include <editor/tools/ToolManager.h>
#include <editor/tools/PlayTool.h>
#include <editor/tools/transform/SelectTool.h>
#include <editor/tools/transform/MoveTool.h>
#include <editor/tools/transform/ScaleTool.h>
#include <editor/tools/transform/RotateTool.h>

#include <QWidget>

namespace ToolManager {
    QWidget* createTools(QToolBar* parent, std::string tabName) {
        QWidget* pageWidget = new QWidget(parent);

        if (tabName == "Home") {
            new Engine::Tools::Select(parent);
            new Engine::Tools::Move(parent);
            new Engine::Tools::Scale(parent);
            new Engine::Tools::Rotate(parent);

            parent->addSeparator();

            new Engine::Tools::Play(parent);

            parent->addSeparator();

            return pageWidget;
        }
    };
}