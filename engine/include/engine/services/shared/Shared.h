#pragma once

#include <datamodel/Instance.h>

namespace Engine {
	class Shared : public Instance {
	public:
		Shared();

		static void properties(ClassDescriptor* desc) {};
	};
}