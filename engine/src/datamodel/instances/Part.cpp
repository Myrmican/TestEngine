#include <datamodel/instances/Part.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_CREATABLE(Part);
	Part::Part() : BasePart("Part") {

	};

	void Part::properties(Engine::ClassDescriptor* desc) {
		Instance::properties(desc);


	}
}