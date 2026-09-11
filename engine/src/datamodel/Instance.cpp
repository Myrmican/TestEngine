#include <datamodel/Instance.h>
#include <scripting/WasmRuntime.h>
#include <datamodel/ClassDescriptor.h>
#include <core/Reflection.h>
#include <datamodel/Property.h>
#include "boost/shared_ptr.hpp"
#include <iostream>
#include <format>
#include <exception>
#include <string>
#include <vector>

namespace Engine {
	Createable::Createable(const std::string name) : Instance(name) {
		
	}

	Instance::Instance(std::string name): parent(nullptr) {
		this->className = name;
		this->name = name;
	}

	void Instance::setName(std::string_view value) {
		if (name == value) return;

		if (value.size() > 100)
			name = std::string(value.substr(0, 100));
		else
			name = std::string(value);

		this->changed.call("Name", name);
	}
	
	void Instance::destroy() {
		removeAllChildren();
		setParent(nullptr);
	}

	void Instance::removeAllChildren() {
		
	}

	std::string Instance::getPath() const {
		if (parent) {
			return parent->getPath() + "/" + std::string(getName());
		}
		return std::string(getName());
	}

	void Instance::setParentInternal(Instance* newParent, bool ignoreLock) {
		std::string message;

		if (internalLocked && !ignoreLock) {
			message = std::format("Attempted to set the parent of {}, but it was locked.", getName());
		}
		
		if (newParent == this) {
			message = std::format("Attempted to parent {} to itself.", getPath());
		}
		else if (this->isAncestorOf(newParent)) {
			message = std::format("Attempted to set a descendant of {} as its parent.", getName());
		}

		if (!message.empty()) throw std::runtime_error(message);

		Instance* oldParent = getParent();

		this->parent = newParent;

		newParent->children.push_back(shared_from_this());

		if (oldParent) {
			std::erase(oldParent->children, shared_from_this());
		}

		if (newParent != NULL) {
			newParent->onChildAdded(this);
		};
	}

	std::shared_ptr<Instance> Instance::clone() {
		std::shared_ptr<Instance> instance = nullptr;
		return instance;
	}

	const std::vector<InstancePtr>& Instance::getDescendants() {
		static std::vector<InstancePtr> result;
		result.clear();

		collectDescendants(this, result);
		return result;
	}

	const std::vector<InstancePtr>& Instance::getDescendants(std::string_view selector) {
		return getDescendants();
	}

	bool Instance::isAncestorOf(const Instance* descendant) const {
		if (!descendant) return false;
		else if (descendant->getParent() == this) return true;

		return isAncestorOf(descendant->getParent());
	}

	bool Instance::isDescendantOf(const Instance* ancestor) {
		if (!ancestor) return false;
		else if (getParent() == ancestor) return true;

		return parent->isDescendantOf(ancestor);
	}

	void Instance::setAttribute(const std::string& name, bool value) {
		attributes[name] = value;
	}

	bool Instance::getAttribute(const std::string& name) {
		auto item = attributes.find(name);
		if (item == attributes.end()) return false;

		return item->second;
	}

	void Instance::BindAPI(WasmRuntime& wasm) {

	}



	void Instance::reflectProperties(ClassDescriptor* desc) {
		auto* classNameProperty = new TypedProperty<Instance, std::string_view>(
			"ClassName",
			"Data",
			&Instance::getClassName,
			nullptr
		);

		desc->addProperty(classNameProperty);

		auto* nameProperty = new TypedProperty<Instance, std::string_view>(
			"Name",
			"Data",
			&Instance::getName,
			&Instance::setName
		);

		desc->addProperty(nameProperty);

		auto* parentProperty = new TypedProperty<Instance, Instance*>(
			"Parent",
			"Data",
			&Instance::getParent,
			&Instance::setParent
		);

		desc->addProperty(parentProperty);
	}
}