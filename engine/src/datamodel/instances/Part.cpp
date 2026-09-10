#include <datamodel/instances/Part.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_CREATEABLE(Part);
	Part::Part() : BasePart("Part") {

	};
}