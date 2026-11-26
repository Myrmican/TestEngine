#include <datamodel/instances/GuiObject.h>

namespace Engine {
	class Frame : public GuiObject {
	public:
		Frame();

		static void properties(ClassDescriptor* desc);
	};
}