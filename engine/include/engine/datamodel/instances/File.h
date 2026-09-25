#include <datamodel/Instance.h>

namespace Engine {
	class File : public Creatable {
	public:
		std::string content;

		File();

		void setContent(std::string& text);

		static void registerProperties(ClassDescriptor* desc) {};
	};
}