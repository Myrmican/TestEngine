#include <editor/tools/Tool.h>

namespace Engine::Tools {
	class Move : public StudioTool {
	public:
		Move(QToolBar* parent) : StudioTool(parent, "Move") {

			connect(this, &QToolButton::toggled, this, [this]() {
				
				});
		};
	};
}