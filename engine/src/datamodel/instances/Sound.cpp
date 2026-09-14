#include <datamodel/instances/Sound.h>
#include <core/Reflection.h>

#include <string>

namespace Engine {
	REGISTER_CREATABLE(Sound);
	Sound::Sound() : Creatable("Sound") {

	}
}