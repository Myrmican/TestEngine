#pragma once

#include <datamodel/Instance.h>
#include <string>

namespace Engine {
	class GuiObject : public Instance {
	public:
		GuiObject(std::string name);

		bool clipsToBounds() const { return m_clipsToBounds;  }
		void setClipsToBounds(const bool clipsToBounds);

		bool isVisible() const { return m_visible; }
		void setVisible(const bool visible);

		float getRotation() const { return m_rotation; }
		void setRotation(const float rotation);

		static void properties(ClassDescriptor* desc);

	private:
		bool m_clipsToBounds = false;
		bool m_visible = true;
		float m_rotation = 0;
	};
}