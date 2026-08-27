#include <datamodel/Instance.h>
#include <scripting/WasmRuntime.h>
#include "boost/shared_ptr.hpp"

#include <format>
#include <exception>
#include <string>

namespace Engine {
	Createable::Createable(const std::string name) : Instance(name) {
		
	}

	Instance::Instance(std::string name): parent(nullptr) {
		this->className = name;
		this->name = name;
	}

	void Instance::setName(std::string_view value) {
		name = std::string(value);
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

	void Instance::setParentInternal(Instance* instance) {
		if (internalLocked) {
			std::string message = std::format("Attempted to set the parent of {}, but it was locked.", getName());
			throw std::runtime_error(message);
		}
		
		if (instance == this) {
			std::string message = std::format("Attempted to parent {} to itself.", getPath());
			throw std::runtime_error(message);
		}
		else if (this->isAncestorOf(instance)) {
			std::string message = std::format("Attempted to set a descendant of {} as its parent.", getName());
			throw std::runtime_error(message);
		}

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

	void Instance::BindAPI(WasmRuntime& wasm) {

	}
}