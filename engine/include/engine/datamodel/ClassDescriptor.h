#pragma once

#include <datamodel/Property.h>
#include <string>
#include <vector>
#include <unordered_map>

namespace Engine {
	class ClassDescriptor {
		std::string m_className;
		ClassDescriptor* m_superClass;

		std::vector<std::unique_ptr<Property>> m_properties;
		std::unordered_map<std::string, Property*> m_propertyMap;

	public:
		ClassDescriptor(std::string className, ClassDescriptor* superClass = nullptr)
			: m_className(std::move(className)), m_superClass(superClass) {}

		~ClassDescriptor() = default;

		void addProperty(Property* prop) {
			if (!prop) return;

			m_properties.push_back(std::unique_ptr<Property>(prop));
			m_propertyMap[prop->m_name] = prop;
		}

		std::vector<const Property*> getAllProperties() const {
			std::vector<const Property*> allProps;

			if (m_superClass) {
				allProps = m_superClass->getAllProperties();
			}

			for (const auto& prop : m_properties) {
				allProps.push_back(prop.get());
			}

			return allProps;
		}
	};
}