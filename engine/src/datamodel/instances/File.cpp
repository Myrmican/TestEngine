#include <datamodel/instances/File.h>
#include <core/Reflection.h>
#include <string>

namespace Engine {
	REGISTER_CREATEABLE(File);
	File::File() : Createable("File") {

	}

	void File::setContent(std::string& text) {
		content = text;
	}
}