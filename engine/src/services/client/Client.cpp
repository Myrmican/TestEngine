#include <services/client/Client.h>
#include <string>

namespace Engine {

	Client::Client() : Instance("Client") {
		internalLocked = true;
	}
}