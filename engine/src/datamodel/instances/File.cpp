#include <datamodel/instances/File.h>

#include <string>

namespace Engine {
	void File::setContent(std::string& text) {
		content = text;
	}
}