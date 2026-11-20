#include <serialization/PlaceDeserializer.h>
#include <datamodel/Instance.h>
#include <datamodel/DataModel.h>
#include <pugixml.hpp>
#include <core/Reflection.h>
#include <string>

static bool processChildren(pugi::xml_node rootNode, Engine::InstancePtr parent) {
	for (pugi::xml_node node : rootNode.children()) {
		if (node.type() != pugi::node_element) {
			continue;
		}

		pugi::xml_attribute className = node.attribute("class");
		if (className) {
			Engine::InstancePtr newInstance = Engine::CreateInstance(className.value());
			if (newInstance) {
				newInstance->setLockedParent(parent.get());

				processChildren(node, newInstance);
			}
		}

	}

	return true;
}

namespace Engine {

	bool PlaceDeserializer::loadFromFile(std::string_view placeData, Engine::DataModel* dataModel) {
		pugi::xml_document document;
		pugi::xml_parse_result result = document.load_string(placeData.data());
		std::cout << placeData;
		if (!result) {
			return false;
		}

		pugi::xml_node root = document.child("test");

		processChildren(root, dataModel->shared_from_this());

		return true;
	}
}