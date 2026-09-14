#include <datamodel/instances/PlayerTemplate.h>
#include <datamodel/instances/Backpack.h>

namespace Engine {
	PlayerTemplate::PlayerTemplate() : Instance("PlayerTemplate") {
		internalLocked = true;

		auto defaultBackpack = std::make_unique<Backpack>();
		addChild(std::move(defaultBackpack));
	}

	
}