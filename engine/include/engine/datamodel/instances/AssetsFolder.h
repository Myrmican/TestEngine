#pragma once

#include <datamodel/instances/Folder.h>

namespace Engine {
	class AssetsFolder : public Folder {
	public:
		AssetsFolder();

		static void properties(ClassDescriptor* desc) {};
	};
}