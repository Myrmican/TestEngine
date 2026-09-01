#include <datamodel/instances/Folder.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_CLASS(Folder);
	Folder::Folder() : Createable("Folder") {

	}
}