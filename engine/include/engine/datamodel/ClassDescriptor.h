#pragma once

#include <datamodel/Property.h>
#include <string>
#include <vector>
#include <unordered_map>

namespace Engine {
	class ClassDescriptor;
	ClassDescriptor* GetClassDescriptor(const std::string& className);

	class ClassDescriptor {
		std::string m_className;
		std::string m_superClassName;
		mutable ClassDescriptor* m_superClass = nullptr;

		std::vector<std::unique_ptr<Property>> m_properties;
		std::unordered_map<std::string, Property*> m_propertyMap;

	public:
		bool isEditorVisible = true;

		ClassDescriptor(std::string className, std::string superClassName, bool isEditorVisible = true)
			: m_className(std::move(className)),
			m_superClassName(std::move(superClassName)),
			isEditorVisible(isEditorVisible) {
		}

		void addProperty(Property* prop) {
			if (!prop) return;

			m_properties.push_back(std::unique_ptr<Property>(prop));
			m_propertyMap[prop->m_name] = prop;
		}

		ClassDescriptor* getSuperClass() const {
			if (!m_superClass && !m_superClassName.empty())
				m_superClass = GetClassDescriptor(m_superClassName);
			return m_superClass;
		}

		std::vector<const Property*> getAllProperties() const {
			std::vector<const Property*> allProps;

			for (const auto& prop : m_properties) {
				allProps.push_back(prop.get());
			}

			if (auto* super = getSuperClass()) {
				auto superProps = super->getAllProperties();
				allProps.insert(allProps.end(), superProps.begin(), superProps.end());
			}

			return allProps;
		}
	};
}