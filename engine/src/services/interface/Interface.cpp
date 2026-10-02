#include <services/interface/Interface.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_INSTANCE(Interface, Instance);
	Interface::Interface() : Instance("Interface") {
		internalLocked = true;
		

	}
}