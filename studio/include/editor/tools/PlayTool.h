#include <editor/tools/Tool.h>

namespace Engine::Tools {
	class Play : public StudioTool {
	public:
		Play(QToolBar* parent) : StudioTool(parent, "Play") {
			this->setCheckable(false);
			
			connect(this, &QToolButton::clicked, this, [this]() {
				this->setText("Stop");
				});
		};
	};
}