#include <services/world/Environment.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_INSTANCE(Environment, Instance);
	Environment::Environment() : Instance("Environment") {
		internalLocked = true;

		
	}

	void Environment::properties(ClassDescriptor* desc) {
		
	}
}