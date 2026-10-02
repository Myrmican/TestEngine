#pragma once

#include <datamodel/Instance.h>
#include <string>

namespace Engine {
	class File : public Creatable {
	public:
		std::string content;

		File();

		void setContent(std::string& text);
		std::string getContent();

		static void properties(ClassDescriptor* desc) {};
	};
}