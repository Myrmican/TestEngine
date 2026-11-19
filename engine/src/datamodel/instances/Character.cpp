#include <datamodel/instances/Character.h>
#include <datamodel/instances/Model.h>
#include <datamodel/instances/Tool.h>
#include <datamodel/instances/Backpack.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_CREATABLE(Character, Instance);
	Character::Character() : Creatable("Character") {

	}

	double Character::getHealth() const {
		return m_health;
	}

	void Character::setHealth(double health) {
		changing.call("Health", health);

		if (changing.isCancelled()) {
			return;
		}

		m_health = health;

		changed.call("Health");
	}

	//If false or nothing is passed in, it will unequip all tools and equip this tool.
	//If true is passed in, it wil just equip this tool in the left hand and keep the current tool in the right hand.
	void Character::equipTool(Tool* tool, bool leftHand) {
		tool->equipping.call();

		if (tool->equipping.isCancelled()) {
			return;
		}

		tool->setParent(this);

		tool->equipped.call();
	}

	void Character::unequipTools() {
		for (const auto& child : getChildren()) {
			if (child->getClassName() == "Tool") {
				Tool* tool = static_cast<Tool*>(child.get());

				tool->unequipping.call();

				if (tool->unequipping.isCancelled()) {
					return;
				}

				//tool->setParent(backpack);

				tool->unequipped.call();
			}
		}
	}

	void Character::properties(ClassDescriptor* desc) {
		desc->addProperty(new TypedProperty<Character, double>(
			"Health",
			"Data",
			&Character::getHealth,
			&Character::setHealth
		));
	}
}