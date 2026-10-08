#include <editor/tools/Tool.h>

namespace Engine::Tools {
	class Scale : public StudioTool {
	public:
		Scale(QToolBar* parent) : StudioTool(parent, "Scale") {

			connect(this, &QToolButton::toggled, this, [this]() {
				
				});
		};
	};
}