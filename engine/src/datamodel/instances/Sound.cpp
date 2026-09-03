#include <datamodel/instances/Sound.h>
#include <core/Reflection.h>

#include <string>

namespace Engine {
	REGISTER_CLASS(Sound);
	Sound::Sound() : Createable("Sound") {

	}
}