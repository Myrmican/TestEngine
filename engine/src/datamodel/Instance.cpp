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
		wasm.registerFunction("Instance_IsAncestorOf", [](int32_t selfHandle, int32_t descendantHandle) -> int32_t {
			Instance* self = InstanceUtil::Resolve(selfHandle);
			Instance* descendant = InstanceUtil::Resolve(descendantHandle);
			if (!self) return 0;

			return self->isAncestorOf(descendant) ? 1 : 0;
			});

		wasm.registerFunction("Instance_GetPath", [](int32_t selfHandle) -> /* something wasm can consume */ {
			Instance* self = InstanceUtil::Resolve(selfHandle);
			if (!self) return;

			return self->getPath();
		});
	}
}