#include <datamodel/instances/Frame.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_CREATABLE(Frame, Instance);
	Frame::Frame() : Creatable("Frame") {

	}
}