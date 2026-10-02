#include <datamodel/instances/SourceFolder.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_INSTANCE(SourceFolder, Folder);
	SourceFolder::SourceFolder() : Folder("Source") {
		internalLocked = true;
	}

}