#include <datamodel/instances/SpawnPoint.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_CREATABLE(SpawnPoint, BasePart);
	SpawnPoint::SpawnPoint() : BasePart("SpawnPoint") {

	}

	void SpawnPoint::properties(Engine::ClassDescriptor* desc) {
		desc->addProperty(new TypedProperty<SpawnPoint, double>(
			"SpawnProtection",
			"Spawning",
			&SpawnPoint::getSpawnProtection,
			&SpawnPoint::setSpawnProtection
		));
		desc->addProperty(new TypedProperty<SpawnPoint, Team*>(
			"Team",
			"Spawning",
			&SpawnPoint::getTeam,
			&SpawnPoint::setTeam
		));
	}
}