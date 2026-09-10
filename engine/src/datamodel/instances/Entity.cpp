#include <datamodel/instances/Entity.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_CREATEABLE(Entity);
	Entity::Entity() : Createable("Entity") {

	}
}