#include <datamodel/Instance.h>

namespace Engine {
	class Frame : public Creatable {
	public:
		Frame();

		static void properties(ClassDescriptor* desc) {};
	};
}