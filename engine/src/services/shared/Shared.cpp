#include <services/shared/Shared.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_INSTANCE(Shared, Instance);
	Shared::Shared() : Instance("Shared") {
		internalLocked = true;
	}
}