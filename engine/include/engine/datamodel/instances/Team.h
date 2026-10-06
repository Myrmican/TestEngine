#pragma once

#include <datamodel/Instance.h>
#include <core/math/Color3.h>

namespace Engine {
	class Team : public Creatable {
	public:
		Team();

		Color3 getColor() const { return m_color; }

		void setColor(const Color3 color) {
			if (m_color != color) {
				changing.call("Color", color);
				m_color = color;
				changed.call("Color");

			}
		}

		static void properties(Engine::ClassDescriptor* desc);
	private:
		Color3 m_color;
	};
}