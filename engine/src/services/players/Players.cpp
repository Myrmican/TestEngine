#include <services/players/Players.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_INSTANCE(Players);
	Players::Players() : Instance("Players") {
		internalLocked = true;
	}
}