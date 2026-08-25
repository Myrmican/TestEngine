#include <datamodel/Instance.h>

namespace Engine {
	class File : public Createable {
	public:
		std::string content;

		File();

		void setContent(std::string& text);
	};
}