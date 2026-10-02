#pragma once

#include <datamodel/instances/Folder.h>

namespace Engine {
	class SourceFolder : public Folder {
	public:
		SourceFolder();

		static void properties(ClassDescriptor* desc) {};
	};
}