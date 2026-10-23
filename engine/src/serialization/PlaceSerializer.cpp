#include <serialization/PlaceSerializer.h>
#include <datamodel/DataModel.h>
#include <pugixml.hpp>

namespace Engine {
	void PlaceSerializer::loadRBXLX(std::string_view filePath) {
		pugi::xml_document document;
		pugi::xml_parse_result result = document.load_file(filePath.data());

		if (!result) {
			return;
		}

		pugi::xml_node root = document.child("roblox");

		for (pugi::xml_node node = root.first_child(); node; node = node.next_sibling()) {
			std::cout << node.name();
		}
	}

	void PlaceSerializer::loadFromFile(const std::string& filePath) {

	}
}