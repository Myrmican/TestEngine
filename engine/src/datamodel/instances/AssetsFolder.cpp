#include <datamodel/instances/AssetsFolder.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_INSTANCE(AssetsFolder, Folder);
	AssetsFolder::AssetsFolder() : Folder("Assets") {
		internalLocked = true;
	}

}