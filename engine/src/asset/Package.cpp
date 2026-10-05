#include <asset/Package.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_INSTANCE(Package, Instance);
	Package::Package() : Instance("Package") {
		
	}
}