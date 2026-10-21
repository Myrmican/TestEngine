#include <editor/tools/ToolManager.h>
#include <editor/tools/PlayTool.h>
#include <editor/tools/transform/SelectTool.h>
#include <editor/tools/transform/MoveTool.h>
#include <editor/tools/transform/ScaleTool.h>
#include <editor/tools/transform/RotateTool.h>
#include <QHBoxLayout>
#include <QWidget>

namespace ToolManager {
    QWidget* createTools(QStackedWidget* parent, std::string tabName) {
        QWidget* pageWidget = new QWidget(parent);

        if (tabName == "Home") {
            QHBoxLayout* layout = new QHBoxLayout(pageWidget);
            layout->setContentsMargins(4, 2, 4, 2);
            layout->setSpacing(4);

            layout->addWidget(new Engine::Tools::Select(pageWidget));
            layout->addWidget(new Engine::Tools::Move(pageWidget));
            layout->addWidget(new Engine::Tools::Scale(pageWidget));
            layout->addWidget(new Engine::Tools::Rotate(pageWidget));

            //layout->addSpacing(10);

            layout->addWidget(new Engine::Tools::Play(pageWidget));

            layout->addStretch();
        }

        return pageWidget;
    }
}