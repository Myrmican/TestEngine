#include <datamodel/instances/Sound.h>
#include <core/Reflection.h>

#include <string>

namespace Engine {
	REGISTER_CREATABLE(Sound, Instance);
	Sound::Sound() : Creatable("Sound") {

	}
}