#include <datamodel/Instance.h>
#include <datamodel/Instances/Camera.h>

namespace Engine {
	class World : public Instance {
	public:
		World();

		Camera* getCurrentCamera() {
			return currentCamera;
		}

		void setCurrentCamera(Camera* camera) {
			currentCamera = camera;
		}

		static void reflectProperties(ClassDescriptor* desc);

	private:
		Camera* currentCamera;
	};
}