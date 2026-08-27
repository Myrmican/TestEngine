#include <services/audio/Audio.h>

namespace Engine {
	Audio::Audio() : Instance("Audio") {
		internalLocked = true;
	}
}