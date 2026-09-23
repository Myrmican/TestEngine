#include <editor/tools/ToolManager.h>

namespace ToolManager {
    void createTools(QToolBar* parent) {

        new Engine::Tools::Select(parent);
        new Engine::Tools::Play(parent);

        parent->addSeparator();
    };
}