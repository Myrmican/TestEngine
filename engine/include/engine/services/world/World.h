#include <datamodel/Instance.h>
#include <datamodel/instances/Model.h>
#include <datamodel/Instances/Camera.h>

namespace Engine {
	class World : public Model {
	public:
		World();

		Camera* getCurrentCamera() const {
			return currentCamera;
		}

		void setCurrentCamera(Camera* camera) {
			currentCamera = camera;
		}

		//static void properties(ClassDescriptor* desc);

	private:
		Camera* currentCamera = nullptr;
	};
}