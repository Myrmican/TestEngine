#include <network/Players.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_INSTANCE(Players, Instance);
	Players::Players() : Instance("Players") {
		internalLocked = true;
	}

	std::vector<Player*> Players::getPlayers() {
		std::vector<Player*> players;

		auto descendants = getDescendants();

		for (Instance* descendant : descendants) {
			if (descendant->getClassName() != "Player") continue;

			players.push_back(static_cast<Player*>(descendant));
		}

		return players;
	}
}