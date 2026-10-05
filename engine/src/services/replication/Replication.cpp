#include <services/replication/Replication.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_INTERNAL(Replication);
	Replication::Replication() : Instance("Replication") {
		internalLocked = true;
	}

	bool Replication::isReplicated(Instance* instance) {
		bool isServer = false;

		if (!isServer) {
			return false;
		}
		else {
			return true;
		}
	}

	void Replication::setReplication(Instance* instance, bool replicated) {
		bool isServer = false; //Replace with networking logic

		if (!isServer) {
			throw std::runtime_error("Replication can only be set on the server.");
		}
		else {

		}
	};

	void Replication::replicateTo(Instance* instance, std::vector<Player*>& players) {
		bool isServer = false; //Replace with networking logic

		if (!isServer) {
			throw std::runtime_error("Replication can only be set on the server.");
		}
		else {

		}
	};

	std::vector<Player*>& Replication::getTargets(Instance* instance) {
		bool isServer = false; //Replace with networking logic

		static std::vector<Player*> targets;

		if (!isServer) {
			throw std::runtime_error("Can only get replication targets on the server.");
		}
		else {

		}

		return targets;
	};
}