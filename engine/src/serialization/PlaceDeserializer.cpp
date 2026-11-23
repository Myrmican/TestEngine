#include <serialization/PlaceDeserializer.h>
#include <datamodel/Service.h>
#include <datamodel/Instance.h>
#include <datamodel/DataModel.h>
#include <Engine.h>
#include <pugixml.hpp>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include <core/Reflection.h>
#include <string>

using json = nlohmann::json;
using namespace nlohmann::literals;

json load_import_map(const std::filesystem::path& path) {
	std::ifstream file(path);
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open JSON file: " + path.string());
	}
	return json::parse(file);
}

json rbx_import_map = load_import_map("studio/assets/configs/rbx_import_map.json");

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

static bool processRbxlx(pugi::xml_node rootNode, Engine::InstancePtr parent, pugi::xml_node templateRoot) {
	for (pugi::xml_node node : rootNode.children()) {
		if (node.type() != pugi::node_element) {
			continue;
		}

		pugi::xml_attribute defaultClassName = node.attribute("class");

		if (defaultClassName) {
			json::json_pointer ptr("/classes");
			ptr.push_back(defaultClassName.value());

			std::string targetClass = defaultClassName.value();
			if (rbx_import_map.contains(ptr)) {
				targetClass = rbx_import_map[ptr].get<std::string>();
			}

			Engine::ClassDescriptor* desc = Engine::GetClassDescriptor(targetClass);
			if (!desc) continue;

			pugi::xml_node itemProperties = node.child("Properties");
			
			Engine::InstancePtr newInstance = Engine::CreateInstance(defaultClassName.value());
			if (newInstance) {
				for (pugi::xml_node property : itemProperties.children()) {
					if (node.type() != pugi::node_element) {
						continue;
					}

					pugi::xml_attribute propertyName = property.attribute("name");
					std::cout << propertyName.value() << std::endl;

				}

				for (const Engine::Property* prop : desc->getAllProperties()) {

				}

				newInstance->setLockedParent(parent.get());

				processChildren(node, newInstance);
			}
		}
	}

	return true;
}

namespace Engine {

	bool PlaceDeserializer::loadFromFile(std::string_view placeData, Engine::EngineInstance* engine) {
		pugi::xml_document document;
		pugi::xml_parse_result result = document.load_string(placeData.data());

		if (!result) {
			return false;
		}

		pugi::xml_node root = document.child("test");

		for (std::string serviceName : Engine::defaultServices) {
			Engine::InstancePtr serviceInstance = Engine::CreateInstance(serviceName);
			if (serviceInstance) {
				serviceInstance->setLockedParent(engine->getDataModel());
				engine->getProvider()->create(serviceInstance.get());
			}
		}

		return processChildren(root, engine->getDataModel()->shared_from_this());
	}

	bool PlaceDeserializer::loadFromRoblox(std::string_view placeData, std::string_view templateData, Engine::DataModel* dataModel) {
		pugi::xml_document robloxDocument;
		pugi::xml_parse_result robloxResult = robloxDocument.load_string(placeData.data());

		pugi::xml_document defaultTemplateDocument;
		pugi::xml_parse_result defaultTemplateResult = defaultTemplateDocument.load_string(templateData.data());

		if (!robloxDocument || !defaultTemplateDocument) {
			return false;
		}

		pugi::xml_node templateRoot = defaultTemplateDocument.child("test");
		pugi::xml_node root = robloxDocument.child("roblox");

		return processRbxlx(root, dataModel->shared_from_this(), templateRoot);
	}
}