#include <datamodel/instances/Entity.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_CREATABLE(Entity);
	Entity::Entity() : Creatable("Entity") {

	}
}