#include <datamodel/Instance.h>
#include <scripting/WasmRuntime.h>

#include <format>
#include <exception>

namespace Engine {
	bool Instance::setParentInternal(Instance* instance) {
		if (instance == this) {
			std::string message = std::format("Attempt to parent {} to itself.", getPath());
			throw std::runtime_error(message);
		}
		else if (this->isAncestorOf(instance)) {
			std::string message = std::format("Attempt to set a descendant of {} as its parent.", getName());
			throw std::runtime_error(message);
		}
	}

	void Instance::BindAPI(WasmRuntime& wasm) {
		wasm.registerFunction("Instance_IsAncestorOf", [](Instance* self, Instance* descendant) -> bool {
			if (!self) return false;
			return self->isAncestorOf(descendant);
			});

		wasm.registerFunction("Instance_GetPath", [](Instance* self) -> std::string {
			if (!self) return "";
			return self->getPath();
			});
	}
}