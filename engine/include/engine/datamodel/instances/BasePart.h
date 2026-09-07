#include <datamodel/Instance.h>
#include <string>

namespace Engine {
	class BasePart : public Createable {
	public:
		BasePart(std::string className);
	};
}