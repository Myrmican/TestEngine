#include <datamodel/instances/SpawnPoint.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_CREATABLE(SpawnPoint, BasePart);
	SpawnPoint::SpawnPoint() : BasePart("SpawnPoint") {

	}
}