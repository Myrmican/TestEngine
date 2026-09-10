#include <services/audio/Audio.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_INSTANCE(Audio);
	Audio::Audio() : Instance("Audio") {
		internalLocked = true;
	}
}