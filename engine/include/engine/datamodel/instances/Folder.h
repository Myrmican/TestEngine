#pragma once

#include <datamodel/Instance.h>
#include <core/math/Color3.h>
#include <string>

namespace Engine {
	class Folder : public Creatable {
	public:
		Folder(std::string_view name = "Folder");

		Color3 getColor() const { return m_color; }

		void setColor(const Color3 color);

		static void properties(ClassDescriptor* desc);

	private:
		Color3 m_color;
	};
}