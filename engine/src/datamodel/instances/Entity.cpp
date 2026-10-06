#include <datamodel/instances/Entity.h>
#include <datamodel/instances/Model.h>
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

	//If false or nothing is passed in, it will unequip all tools and equip this tool.
	//If true is passed in, it wil just equip this tool in the left hand and keep the current tool in the right hand.
	void Entity::equipTool(Tool* tool, bool leftHand) {
		tool->equipping.call();



		tool->equipped.call();
	}

	void Entity::unequipTools() {
		const Model* character = dynamic_cast<const Model*>(getParent());
		for (const auto& child : character->getChildren()) {
			if (child->getClassName() == "Tool") {
				//child->setParent(backpack);
			}
		}
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