#include <services/world/World.h>
#include <datamodel/instances/Camera.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_INSTANCE(World);
	World::World() : Instance("World") {
		internalLocked = true;

		auto worldCamera = std::make_unique<Camera>();
		this->addChild(std::move(worldCamera));
	}

	void World::reflectProperties(ClassDescriptor* desc) {
		Instance::reflectProperties(desc);

		auto* currentCameraProperty = new TypedProperty<World, Camera*>(
			"CurrentCamera", "Data", &World::getCurrentCamera, &World::setCurrentCamera
		);
		desc->addProperty(currentCameraProperty);
	}
}