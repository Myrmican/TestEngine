#pragma once

#include <datamodel/Instance.h>
#include <string>

namespace Engine {
	class GuiBase : public Instance {
	public:
		GuiBase(std::string name);

		static void properties(ClassDescriptor* desc) {};
	};
}