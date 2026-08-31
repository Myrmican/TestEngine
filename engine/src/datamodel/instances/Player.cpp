#include <datamodel/instances/Player.h>

namespace Engine {
	Player::Player() : Instance("Player") {
		internalLocked = true;
	}
}