#include <services/world/World.h>

namespace Engine {
	World::World() : Instance("World") {
		internalLocked = true;
	}
}