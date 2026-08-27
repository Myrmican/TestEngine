#include <services/players/Players.h>

namespace Engine {
	Players::Players() : Instance("Players") {
		internalLocked = true;
	}
}