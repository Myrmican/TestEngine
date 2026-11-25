#include <editor/tools/Tool.h>

namespace Engine::Tools {
	class Scale : public StudioButtonTool {
	public:
		Scale(QWidget* parent) : StudioButtonTool(parent, "Scale") {

			connect(this, &QToolButton::toggled, this, [this]() {
				
				});
		};
	};
}