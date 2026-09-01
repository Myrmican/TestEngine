#include <datamodel/Instance.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_CLASS(Tool);

	class Tool : public Createable {
	public:
		Tool();
	};
}