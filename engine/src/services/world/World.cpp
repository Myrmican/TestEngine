#include <services/world/World.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_INSTANCE(World);
	World::World() : Instance("World") {
		internalLocked = true;
	}

	void World::reflectProperties(ClassDescriptor* desc) {
		Instance::reflectProperties(desc);

		auto* currentCameraProperty = new TypedProperty<Instance, std::string_view>(
			"CurrentCamera", "Data", &Instance::getName, &Instance::setName
		);
		desc->addProperty(currentCameraProperty);
	}
}