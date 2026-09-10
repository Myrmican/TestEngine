#include <services/shared/Shared.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_CLASS(Shared);
	Shared::Shared() : Instance("Shared") {
		internalLocked = true;
	}
}