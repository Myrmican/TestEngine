#include <datamodel/instances/Overlay.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_CREATABLE(Overlay, OverlayBase);
	Overlay::Overlay(std::string_view name) : OverlayBase(name.data()) {

	}

	void Overlay::properties(ClassDescriptor* desc) {
		
	}

}