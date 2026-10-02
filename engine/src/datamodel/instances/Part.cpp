#include <datamodel/instances/Part.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_CREATABLE(Part, BasePart);
	Part::Part() : BasePart("Part") {

	};

	void Part::properties(Engine::ClassDescriptor* desc) {
		

	}
}