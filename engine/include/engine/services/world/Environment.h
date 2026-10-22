#include <datamodel/Instance.h>

namespace Engine {
	class Environment : public Instance {
	public:
		Environment();

		

		static void properties(ClassDescriptor* desc);

	private:
		Camera* currentCamera = nullptr;
	};
}