#pragma once

#include <datamodel/instances/BasePart.h>

namespace Engine {
	class SpawnPoint : public BasePart {
	public:
		SpawnPoint();

		static void properties(Engine::ClassDescriptor* desc) {};
	};
}