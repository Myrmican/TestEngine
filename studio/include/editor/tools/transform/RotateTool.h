#include <editor/tools/Tool.h>

namespace Engine::Tools {
	class Rotate : public StudioButtonTool {
	public:
		Rotate(QWidget* parent) : StudioButtonTool(parent, "Rotate") {

			connect(this, &QToolButton::toggled, this, [this]() {
				
				});
		};
	};
}