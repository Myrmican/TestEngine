#include <services/client/Client.h>
#include <core/Reflection.h>
#include <string>

namespace Engine {
	REGISTER_CLASS(Client);
	Client::Client() : Instance("Client") {
		internalLocked = true;
	}
}