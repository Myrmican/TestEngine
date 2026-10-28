#pragma once

#include <editor/tools/Tool.h>
#include <project/Project.h>
#include <QToolBar>

namespace Engine::Tools {
	class Play : public StudioTool {
	public:
		Play(QWidget* parent) : StudioTool(parent, "Play") {
			this->setCheckable(false);
			
			connect(this, &QToolButton::clicked, this, [this, parent]() {
				this->setText("Stop");

				Project* project = ProjectManager::getProject(this);
				project->getScriptEngine()->executeFiles();
				});
		};
	};
}