#include <datamodel/Instance.h>

namespace Engine {
	class PlayerTemplate : public Instance {
	public:
		PlayerTemplate();

		static void properties(ClassDescriptor* desc) {};
	};
}