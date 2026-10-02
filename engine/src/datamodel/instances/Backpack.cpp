#include <datamodel/instances/Backpack.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_INSTANCE(Backpack, Instance);
	Backpack::Backpack() : Instance("Backpack") {

	}
}