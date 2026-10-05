#include <services/anticheat/Anticheat.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_INTERNAL(Anticheat, Instance);
	Anticheat::Anticheat() : Instance("Anticheat") {
		internalLocked = true;
	}

	
}