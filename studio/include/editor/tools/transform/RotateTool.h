#include <editor/tools/Tool.h>

namespace Engine::Tools {
	class Rotate : public StudioTool {
	public:
		Rotate(QToolBar* parent) : StudioTool(parent, "Rotate") {

			connect(this, &QToolButton::toggled, this, [this]() {
				
				});
		};
	};
}