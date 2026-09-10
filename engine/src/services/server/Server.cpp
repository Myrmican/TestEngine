#include <services/server/Server.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_CLASS(Server);
	Server::Server() : Instance("Server") {
		internalLocked = true;
	}

	int Server::getPing() const {
		return 1;
	}

	float Server::getPacketLoss() const {
		return 0.5;
	}
}