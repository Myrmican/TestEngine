#include <datamodel/Instance.h>
#include <network/Player.h>

namespace Engine {
	class Players : public Instance {
	public:
		Players();

		Player* getPlayers();
	};
}