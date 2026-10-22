#include <datamodel/instances/GuiBase.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_INSTANCE(GuiBase);
	GuiBase::GuiBase() : Instance("GuiBase") {

	}
}