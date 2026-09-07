#include <datamodel/instances/BasePart.h>
#include <core/Reflection.h>

#include <string>

namespace Engine {
	REGISTER_CLASS(BasePart);
	BasePart::BasePart(std::string className) : Createable("BasePart") {

	}
}