#include <datamodel/instances/Canvas.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_CREATABLE(Canvas, OverlayBase);
	Canvas::Canvas(std::string_view name) : OverlayBase(name.data()) {

	}

	void Canvas::properties(ClassDescriptor* desc) {
		
	}

}