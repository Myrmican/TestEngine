#include <services/world/World.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_CLASS(World);
	World::World() : Instance("World") {
		internalLocked = true;
	}
}