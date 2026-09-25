#include <datamodel/Instance.h>

namespace Engine {
	class Backpack : public Instance {
	public:
		Backpack();

		static void registerProperties(ClassDescriptor* desc) {};
	};
}