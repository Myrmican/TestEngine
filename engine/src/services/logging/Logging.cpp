#include <services/logging/Logging.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_INTERNAL(Logging);
	Logging::Logging() : Instance("Logging") {
		internalLocked = true;
	}

	
}