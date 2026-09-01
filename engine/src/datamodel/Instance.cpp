#include <datamodel/Instance.h>
#include <scripting/WasmRuntime.h>
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

	std::string_view Instance::setName(std::string_view value) {
		if (name == value) return value;

		if (value.size() > 100)
			name = std::string(value.substr(0, 100));
		else
			name = std::string(value);
		return name.get();
	}
	
	void Instance::destroy() {
		removeAllChildren();
		setParent(nullptr);
	}

	void Instance::removeAllChildren() {
		
	}

	std::string Instance::getPath() const {
		if (parent) {
			return parent->getPath() + "/" + getName();		 
		}
		return getName();
	}

	void Instance::setParentInternal(Instance* instance, bool ignoreLock) {
		std::string message;

		if (internalLocked && !ignoreLock) {
			message = std::format("Attempted to set the parent of {}, but it was locked.", getName());
		}
		
		if (instance == this) {
			message = std::format("Attempted to parent {} to itself.", getPath());
		}
		else if (this->isAncestorOf(instance)) {
			message = std::format("Attempted to set a descendant of {} as its parent.", getName());
		}

		if (!message.empty()) throw std::runtime_error(message);

		Instance* oldParent = getParent();

		this->parent = instance;

		instance->children.push_back(shared_from_this());

		if (oldParent) {
			std::erase(oldParent->children, shared_from_this());
		}
	}

	std::shared_ptr<Instance> Instance::clone() {
		std::shared_ptr<Instance> instance = nullptr;
		return instance;
	}

	const std::vector<InstancePtr>& Instance::getDescendants() {
		return children;
	}

	const std::vector<InstancePtr>& Instance::getDescendants(std::string selector) {
		return children;
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
}