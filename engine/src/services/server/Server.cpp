#include <services/server/Server.h>

namespace Engine {
	Server::Server() : Instance("Server") {

	}

	int Server::getPing() const {
		return 1;
	}

	float Server::getPacketLoss() const {
		return 0.5;
	}
}