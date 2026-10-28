#include <datamodel/instances/GuiBase.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_INSTANCE(GuiBase, Instance);
	GuiBase::GuiBase(std::string name) : Instance(name) {

	}
}