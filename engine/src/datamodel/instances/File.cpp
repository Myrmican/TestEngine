#include <datamodel/instances/File.h>

#include <string>

namespace Engine {
	File::File() : Createable("File") {

	}

	void File::setContent(std::string& text) {
		content = text;
	}
}