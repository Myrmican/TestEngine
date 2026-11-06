#include <serialization/PlaceSerializer.h>
#include <datamodel/DataModel.h>
#include <pugixml.hpp>

namespace Engine {
	bool PlaceSerializer::loadRBXLX(std::string_view filePath, DataModel* dataModel) {
		pugi::xml_document document;
		pugi::xml_parse_result result = document.load_file(filePath.data());

		if (!result) {
			return false;
		}

		pugi::xml_node root = document.child("roblox");

		for (pugi::xml_node node = root.first_child(); node; node = node.next_sibling()) {
			std::cout << node.name();
		}

		return true;
	}

	bool PlaceSerializer::loadFromFile(const std::string& filePath, DataModel* dataModel) {
		pugi::xml_document document;
		pugi::xml_parse_result result = document.load_file(filePath.data());

		if (!result) {
			return false;
		}

		pugi::xml_node root = document.child("roblox");

		for (pugi::xml_node node = root.first_child(); node; node = node.next_sibling()) {
			std::cout << node.name();
		}

		return true;
	}
}