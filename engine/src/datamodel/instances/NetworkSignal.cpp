#include <datamodel/instances/NetworkSignal.h>
#include <core/Reflection.h>
#include <Engine.h>

namespace Engine {
	REGISTER_CREATABLE(NetworkSignal);
	NetworkSignal::NetworkSignal() : Creatable("NetworkSignal") {

	}

	void NetworkSignal::Send(Player* target, const std::vector<uint8_t>& payload) {
		const bool isServer = true;
		if (!isServer) {

		}
		else {
			if (target == nullptr) {
				Logging::error("Target has to be specified when sending NetworkSignal from server.");
				Logging::error(this->getPath());
				return;
			}
		}
	}

	void NetworkSignal::Broadcast(const std::vector<uint8_t>& payload) {
		const bool isServer = true; //Replace this with actual networking logic later

		if (!isServer) {
			//Logging::error("Can only broadcast NetworkSignals from the server.");
			//Logging::error(this->getPath());
			return;
		}
		else {

		}
	}
}