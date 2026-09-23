#include <editor/tools/Tool.h>

namespace Engine::Tools {
	class Select : public StudioTool {
	public:
		Select(QToolBar* parent) : StudioTool(parent, "Select") {

			connect(this, &QToolButton::toggled, this, [this]() {
				
				});
		};
	};
}