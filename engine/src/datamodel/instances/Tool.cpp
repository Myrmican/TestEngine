#include <datamodel/instances/Tool.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_CREATEABLE(Tool);
	Tool::Tool() : Createable("Tool") {


	}
}