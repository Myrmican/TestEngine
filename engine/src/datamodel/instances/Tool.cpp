#include <datamodel/instances/Tool.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_CREATABLE(Tool, Instance);
	Tool::Tool() : Creatable("Tool") {


	}
}