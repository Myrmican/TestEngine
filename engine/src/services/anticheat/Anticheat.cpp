#include <services/anticheat/Anticheat.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_INTERNAL(Anticheat);
	Anticheat::Anticheat() : Instance("Anticheat") {
		internalLocked = true;
	}

	
}