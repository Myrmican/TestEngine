#include <datamodel/Instance.h>

namespace Engine {
	class Frame : public Creatable {
	public:
		Frame();

		static void registerProperties(ClassDescriptor* desc) {};
	};
}