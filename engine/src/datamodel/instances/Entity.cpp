#include <datamodel/instances/Entity.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_CREATABLE(Entity, Instance);
	Entity::Entity() : Creatable("Entity") {

	}

	double Entity::getHealth() const {
		return m_health;
	}

	void Entity::setHealth(double health) {
		m_health = health;
	}

	void Entity::properties(ClassDescriptor* desc) {
		desc->addProperty(new TypedProperty<Entity, double>(
			"Health",
			"Data",
			&Entity::getHealth,
			&Entity::setHealth
		));
	}
}