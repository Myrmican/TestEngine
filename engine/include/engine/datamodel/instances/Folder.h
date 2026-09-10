#pragma once

#include <datamodel/Instance.h>
#include <core/math/Color3.h>

namespace Engine {
	class Folder : public Createable {
	public:
		Folder();

		Color3 getColor() const { return m_color; }

		void setColor(const Color3 color) {
			if (m_color != color)
				this->changed.call("Color", m_color);
			m_color = color;
		}

		static void reflectProperties(ClassDescriptor* desc);

	private:
		Color3 m_color;
	};
}