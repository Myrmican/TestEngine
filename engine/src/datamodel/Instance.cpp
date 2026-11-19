#include <datamodel/Instance.h>
#include <scripting/WasmRuntime.h>
#include <datamodel/ClassDescriptor.h>
#include <core/Reflection.h>
#include <datamodel/DataModel.h>
#include <datamodel/Property.h>
#include <iostream>
#include <format>
#include <exception>
#include <string>
#include <vector>
#include <algorithm>

namespace Engine {
	Creatable::Creatable(const std::string name) : Instance(name) {
		
	}

	REGISTER_INSTANCE(Instance, );
	Instance::Instance(std::string name) : parent(nullptr) {
		this->className = name;
		this->name = name;
	}

	void Instance::setName(std::string_view value) {
		if (name == value) return;

		changing.call("Name", value);

		if (changing.isCancelled()) {
			return;
		}

		if (value.size() > 100)
			name = std::string(value.substr(0, 100));
		else
			name = std::string(value);

		changed.call("Name");
	}

	void Instance::destroy() {
		removeAllChildren();
		setParent(nullptr);
	}

	void Instance::removeAllChildren() {
		for (auto& child : children) {
			child->setParent(nullptr);
		}
		children.clear();
	}

	std::string Instance::getPath() const {
		if (parent && parent->getClassName() == "DataModel") {
			return std::string(getName());
		}

		if (parent) {
			return parent->getPath() + "/" + std::string(getName());
		}
		return std::string(getName());
	}

	bool Instance::setParentInternal(Instance* newParent, bool ignoreLock) {
		if (parent == newParent) return true;

		std::string message;
		if (internalLocked && !ignoreLock) {
			message = std::format("Attempted to set the parent of {}, but it was locked.", getName());
		}
		else if (newParent == this) {
			message = std::format("Attempted to parent {} to itself.", getPath());
		}
		else if (newParent && this->isAncestorOf(newParent)) {
			message = std::format("Attempted to set a descendant of {} as its parent.", getName());
		}
		if (!message.empty()) throw std::runtime_error(message);

		// Keeps us alive while we're detached from the old parent.
		// Throws std::bad_weak_ptr if this wasn't created with make_shared.
		InstancePtr self = shared_from_this();

		// Detach from the old parent (nothing to do for a fresh instance)
		if (parent) {
			auto& siblings = parent->children;
			siblings.erase(std::remove(siblings.begin(), siblings.end(), self), siblings.end());
		}

		// Attach to the new parent (nullptr just leaves us detached)
		parent = newParent;
		if (newParent) {
			newParent->children.push_back(self);
			newParent->onChildAdded(this);
		}

		changed.call("Parent");
		return true;
	}

	InstancePtr Instance::clone() {
		return nullptr;
	}

	std::vector<Instance*> Instance::getDescendants() {
		std::vector<Instance*> result;
		collectDescendants(this, result);
		return result;
	}

	std::vector<Instance*> Instance::getDescendants(std::string_view selector) {
		return getDescendants();
	}

	Instance* Instance::findChild(std::string name) const {

		const std::vector<InstancePtr>& children = getChildren();

		for (const InstancePtr& child : children) {
			Instance* childPtr = child.get();
			if (childPtr->getName() != name) continue;

			return childPtr;
		}

		return nullptr;
	}

	Instance* Instance::findChildClass(std::string className) const {

		const std::vector<InstancePtr>& children = getChildren();

		for (const InstancePtr& child : children) {
			Instance* childPtr = child.get();
			if (childPtr->getClassName() != className) continue;
			
			return childPtr;
		}

		return nullptr;
	}

	bool Instance::isAncestorOf(const Instance* descendant) const {
		if (!descendant) return false;
		else if (descendant->getParent() == this) return true;

		return isAncestorOf(descendant->getParent());
	}

	bool Instance::isDescendantOf(const Instance* ancestor) {
		if (!ancestor) return false;
		else if (getParent() == ancestor) return true;
		if (!parent) return false;

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

	void Instance::properties(ClassDescriptor* desc) {
		auto* classNameProperty = new TypedProperty<Instance, std::string_view>(
			"ClassName", "Data", &Instance::getClassName, nullptr
		);
		desc->addProperty(classNameProperty);

		auto* nameProperty = new TypedProperty<Instance, std::string_view>(
			"Name", "Data", &Instance::getName, &Instance::setName
		);
		desc->addProperty(nameProperty);

		auto* parentProperty = new TypedProperty<Instance, Instance*>(
			"Parent", "Data", &Instance::getParent, &Instance::setParent
		);
		desc->addProperty(parentProperty);
	}
}