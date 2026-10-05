#include <datamodel/Instance.h>

namespace Engine {
	class Audio : public Instance {
	public:
		Audio();

		static void properties(ClassDescriptor* desc) {};
	};
}