#include <serialization/PlaceDeserializer.h>
#include <datamodel/DataModel.h>
#include <pugixml.hpp>

namespace Engine {

	bool PlaceDeserializer::loadFromFile(std::string_view placeData, DataModel* dataModel) {
		pugi::xml_document document;
		pugi::xml_parse_result result = document.load_string(placeData.data());

		if (!result) {
			return false;
		}

		pugi::xml_node root = document.child("test");

		for (pugi::xml_node node : root.children()) {
			if (node.type() != pugi::node_element) {
				continue;
			}

			pugi::xml_attribute attr = node.attribute("id");
			if (attr) {
				std::cout << attr.name() << " = " << attr.value() << '\n';
			}
		}

		return true;
	}
}