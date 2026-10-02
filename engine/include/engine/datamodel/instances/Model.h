#include <datamodel/Instance.h>
#include <string>

namespace Engine {
	class Model : public Creatable {
	public:
		Model(std::string name = "Model");

		static void properties(ClassDescriptor* desc);
	};
}