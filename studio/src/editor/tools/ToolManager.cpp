#include <editor/tools/ToolManager.h>

namespace ToolManager {
    void createTools(QToolBar* parent) {

        new Engine::Tools::Select(parent);
        new Engine::Tools::Move(parent);
        new Engine::Tools::Scale(parent);
        new Engine::Tools::Rotate(parent);

        parent->addSeparator();

        new Engine::Tools::Play(parent);

        parent->addSeparator();
    };
}