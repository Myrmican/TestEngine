#include <datamodel/Instance.h>
#include <network/Player.h>
#include <vector>

namespace Engine {
	class Players : public Instance {
	public:
		Players();

		std::vector<Player*> getPlayers();

		static void properties(ClassDescriptor* desc) {};
	};
}