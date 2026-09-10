#include <services/interface/Interface.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_CLASS(Interface);
	Interface::Interface() : Instance("Interface") {
		internalLocked = true;
		

	}
}