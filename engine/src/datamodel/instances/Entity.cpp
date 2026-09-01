#include <datamodel/instances/Entity.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_CLASS(Entity);
	Entity::Entity() : Createable("Entity") {

	}
}