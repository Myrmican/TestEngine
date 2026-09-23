#include <datamodel/instances/NetworkSignal.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_CREATABLE(NetworkSignal);
	NetworkSignal::NetworkSignal() : Creatable("NetworkSignal") {

	}

	void NetworkSignal::Send(Player* target, const std::vector<uint8_t>& payload) {

	}

	void NetworkSignal::Broadcast(const std::vector<uint8_t>& payload) {
		const isServer = true; //Replace this with actual networking logic later

		if (!isServer) {
			//Logging::error("Can only broadcast NetworkSignals from the server.");
			return;
		}
		else {

		}
	}
}