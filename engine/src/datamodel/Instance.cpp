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

	Instance::Instance(std::string name) : parent(nullptr) {
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
		setParent(nullptr); // after this returns, `this` has been deleted — don't touch it again
	}

	void Instance::removeAllChildren() {
		children.clear(); // unique_ptr cascade: this recursively destroys all descendants too
	}

	std::string Instance::getPath() const {
		if (parent) {
			return parent->getPath() + "/" + std::string(getName());
		}
		return std::string(getName());
	}

	void Instance::addChild(InstancePtr child) {
		if (!child) return;

		Instance* rawChild = child.get();
		rawChild->parent = this;
		children.push_back(std::move(child));
		onChildAdded(rawChild);
	}

	bool Instance::setParentInternal(Instance* newParent, bool ignoreLock) {
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

		Instance* oldParent = getParent();
		this->parent = newParent;

		// Move ownership out of the old parent's children vector.
		InstancePtr self;
		if (oldParent) {
			auto& siblings = oldParent->children;
			auto it = std::find_if(siblings.begin(), siblings.end(),
				[this](const InstancePtr& p) { return p.get() == this; });
			if (it != siblings.end()) {
				self = std::move(*it);
				siblings.erase(it);
			}
		}

		if (newParent) {
			// `self` must be valid here — this instance must already have been
			// owned by an old parent (or use addChild() for brand-new instances).
			if (self) {
				newParent->children.push_back(std::move(self));
			}
			newParent->onChildAdded(this);
		}

		return true;
	}

	std::unique_ptr<Instance> Instance::clone() {
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

	void Instance::setParent(Instance* instance, bool ignoreLock) {
		setParentInternal(instance, ignoreLock);
	}

	void Instance::registerProperties(ClassDescriptor* desc) {
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