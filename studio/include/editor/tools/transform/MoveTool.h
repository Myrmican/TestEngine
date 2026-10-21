#include <editor/tools/Tool.h>

namespace Engine::Tools {
	class Move : public StudioTool {
	public:
		Move(QWidget* parent) : StudioTool(parent, "Move") {

			connect(this, &QToolButton::toggled, this, [this]() {
				
				});
		};
	};
}