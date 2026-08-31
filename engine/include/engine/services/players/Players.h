#include <datamodel/Instance.h>
#include <datamodel/instances/Player.h>

namespace Engine {
	class Players : public Instance {
	public:
		Players();

		Player* getPlayers();
	};
}