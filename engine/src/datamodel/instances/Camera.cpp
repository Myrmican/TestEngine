#include <datamodel/instances/Camera.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_CLASS(Camera);
	Camera::Camera() : Createable("Camera") {

	}
}