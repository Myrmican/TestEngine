#include <services/world/World.h>
#include <datamodel/instances/Camera.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_INSTANCE(World, Model);
	World::World() : Model("World") {
		internalLocked = true;

		auto worldCamera = std::make_unique<Camera>();
		this->addChild(std::move(worldCamera));
	}

	void World::properties(ClassDescriptor* desc) {
		auto* currentCameraProperty = new TypedProperty<World, Camera*>(
			"CurrentCamera", "Data", &World::getCurrentCamera, &World::setCurrentCamera
		);
		desc->addProperty(currentCameraProperty);
	}
}