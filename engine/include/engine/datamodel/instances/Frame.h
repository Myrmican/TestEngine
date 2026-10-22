#include <datamodel/instances/GuiBase.h>

namespace Engine {
	class Frame : public GuiBase, public Creatable {
	public:
		Frame();

		static void properties(ClassDescriptor* desc) {};
	};
}