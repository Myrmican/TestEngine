#include <datamodel/Instance.h>
#include <network/Player.h>
#include <vector>

namespace Engine {
	class Replication : public Instance {
	public:
		Replication();

		bool isReplicated(Instance* instance);
		void setReplication(Instance* instance, bool replicated);
		void replicateTo(Instance* instance, std::vector<Player*>& players);
		std::vector<Player*>& getTargets(Instance* instance);
	};
}