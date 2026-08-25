#include <datamodel/Instance.h>
#include <scripting/WasmRuntime.h>

#include <format>
#include <exception>
#include <string>

namespace Engine {
	Createable::Createable(const std::string name) : Instance(name) {
		
	}

	bool Instance::setParentInternal(Instance* instance) {
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
	}
}