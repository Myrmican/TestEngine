#include <datamodel/Instance.h>

namespace Engine {
	class Sound : public Creatable {
	public:
		Sound();

		void play();
		void pause();
		void stop();

		static void registerProperties(ClassDescriptor* desc) {};
	};
}