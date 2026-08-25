#include <services/server/Server.h>

namespace Engine {
	Server::Server() : Instance("Server") {
		
	}

	int Server::getPing() const {
		return 1;
	}

	float Server::getPacketLoss() {
		return 0.5;
	}
}