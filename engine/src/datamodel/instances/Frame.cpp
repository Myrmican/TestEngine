#include <datamodel/instances/Frame.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_CREATEABLE(Frame);
	Frame::Frame() : Createable("Frame") {

	}
}