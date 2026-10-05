#include <datamodel/Instance.h>

namespace Engine {
	class Interface : public Instance {
	public:
		Interface();

		static void properties(ClassDescriptor* desc) {};
	};
}