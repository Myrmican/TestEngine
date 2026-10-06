#pragma once

#include <datamodel/Instance.h>
#include <core/Event.h>
#include <core/Reflection.h>

namespace Engine {
	class Tool : public Creatable {
	public:

		Event<> equipping;
		Event<> unequipping;

		Event<> equipped;
		Event<> unequipped;

		Tool();

		static void properties(ClassDescriptor* desc) {};
	};
}