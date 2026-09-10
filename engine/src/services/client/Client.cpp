#include <services/client/Client.h>
#include <core/Reflection.h>
#include <string>

namespace Engine {
	REGISTER_INSTANCE(Client);
	Client::Client() : Instance("Client") {
		internalLocked = true;
	}
}