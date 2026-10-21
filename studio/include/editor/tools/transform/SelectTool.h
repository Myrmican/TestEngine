#pragma once

#include <editor/tools/Tool.h>

namespace Engine::Tools {
	class Select : public StudioTool {
	public:
		Select(QWidget* parent) : StudioTool(parent, "Select") {

			connect(this, &QToolButton::toggled, this, [this]() {
				
				});
		};
	};
}