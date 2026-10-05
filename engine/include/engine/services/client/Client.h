#pragma once

#include <datamodel/Instance.h>

namespace Engine {
	class Client : public Instance {
	public:
		Client();

		static void properties(ClassDescriptor* desc) {};
	};
}