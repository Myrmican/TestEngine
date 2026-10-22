#include <datamodel/Instance.h>
#include <string>

namespace Engine {
	class GuiBase : public Instance {
	public:
		GuiBase();

		static void properties(ClassDescriptor* desc) {};
	};
}