#include <datamodel/instances/Tool.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_CREATABLE(Tool);
	Tool::Tool() : Creatable("Tool") {


	}
}