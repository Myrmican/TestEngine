#include <datamodel/instances/Part.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_CREATABLE(Part);
	Part::Part() : BasePart("Part") {

	};

	void Part::reflectProperties(Engine::ClassDescriptor* desc) {
		Instance::reflectProperties(desc);


	}
}