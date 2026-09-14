#include <datamodel/instances/File.h>
#include <core/Reflection.h>
#include <string>

namespace Engine {
	REGISTER_CREATABLE(File);
	File::File() : Creatable("File") {

	}

	void File::setContent(std::string& text) {
		content = text;
	}
}