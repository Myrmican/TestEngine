#include <datamodel/Instance.h>

namespace Engine {
	class Entity : public Creatable {
	public:
		Entity();

		static void registerProperties(ClassDescriptor* desc) {};
	};
}