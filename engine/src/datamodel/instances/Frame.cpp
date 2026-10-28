#include <datamodel/instances/Frame.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_CREATABLE(Frame, GuiBase);
	Frame::Frame() : GuiBase("Frame"), Creatable("Frame") {

	}
}