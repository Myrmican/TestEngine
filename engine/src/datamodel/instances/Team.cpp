#include <datamodel/instances/Team.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_CREATABLE(Team, Instance);
	Team::Team() : Creatable("Team") {


	}

	void Team::properties(Engine::ClassDescriptor* desc) {
		desc->addProperty(new TypedProperty<Team, Color3>(
			"Color",
			"Appearance",
			&Team::getColor,
			&Team::setColor
		));
	}
}