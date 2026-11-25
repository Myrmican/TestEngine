#pragma once

#include <editor/tools/Tool.h>

namespace Engine::Tools {
	class Select : public StudioButtonTool {
	public:
		Select(QWidget* parent) : StudioButtonTool(parent, "Select") {

			connect(this, &QToolButton::toggled, this, [this]() {
				
				});
		};
	};
}