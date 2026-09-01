#include <datamodel/instances/Part.h>
#include <core/Reflection.h>

#include <string>

namespace Engine {
	REGISTER_CLASS(Part);
	Part::Part() : Createable("Part") {

	}
}