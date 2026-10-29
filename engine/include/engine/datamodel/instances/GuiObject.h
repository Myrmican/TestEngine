#pragma once

#include <datamodel/Instance.h>
#include <string>

namespace Engine {
	class GuiObject : public Instance {
	public:
		GuiObject(std::string name);

		bool clipsToBounds() const { return m_clipToBounds;  }
		void setClipToBounds(const bool clipToBounds);

		bool isVisible() const { return m_visible; }
		void setVisible(const bool visible);

		static void properties(ClassDescriptor* desc);

	private:
		bool m_clipToBounds = false;
		bool m_visible = true;
	};
}