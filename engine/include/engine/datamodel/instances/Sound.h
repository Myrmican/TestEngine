#include <datamodel/Instance.h>

namespace Engine {
	class Sound : public Createable {
	public:
		Sound();

		void play();
		void pause();
		void stop();
	};
}