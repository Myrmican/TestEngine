#include <editor/tools/Tool.h>

namespace Engine::Tools {
	class Rotate : public StudioTool {
	public:
		Rotate(QWidget* parent) : StudioTool(parent, "Rotate") {

			connect(this, &QToolButton::toggled, this, [this]() {
				
				});
		};
	};
}