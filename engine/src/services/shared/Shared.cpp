#include <services/shared/Shared.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_INSTANCE(Shared);
	Shared::Shared() : Instance("Shared") {
		internalLocked = true;
	}
}