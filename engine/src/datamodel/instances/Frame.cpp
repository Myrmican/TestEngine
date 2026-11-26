#include <datamodel/instances/Frame.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_CREATABLE(Frame, GuiObject);
	Frame::Frame() : GuiObject("Frame") {

	}

	void Frame::properties(ClassDescriptor* desc) {
		
	}
}