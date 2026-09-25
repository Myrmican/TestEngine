#include <datamodel/instances/PlayerTemplate.h>
#include <datamodel/instances/Backpack.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_INSTANCE(PlayerTemplate);
	PlayerTemplate::PlayerTemplate() : Instance("PlayerTemplate") {
		internalLocked = true;

		auto defaultBackpack = std::make_unique<Backpack>();
		addChild(std::move(defaultBackpack));
	}

	
}