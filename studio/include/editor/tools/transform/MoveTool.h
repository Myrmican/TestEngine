#include <editor/tools/Tool.h>

namespace Engine::Tools {
	class Move : public StudioButtonTool {
	public:
		Move(QWidget* parent) : StudioButtonTool(parent, "Move") {

			connect(this, &QToolButton::toggled, this, [this]() {
				
				});
		};
	};
}