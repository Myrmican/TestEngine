#include <datamodel/instances/GuiBase.h>

namespace Engine {
	class Frame : public GuiBase {
	public:
		Frame();

		static void properties(ClassDescriptor* desc) {};
	};
}