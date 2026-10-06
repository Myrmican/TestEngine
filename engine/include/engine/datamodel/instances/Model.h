#include <datamodel/Instance.h>
#include <string>

namespace Engine {
	class Model : public Creatable {
	public:
		Model(std::string name = "Model");

		void pivotTo();

		static void properties(ClassDescriptor* desc);
	};
}