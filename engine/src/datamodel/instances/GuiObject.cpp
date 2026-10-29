#include <datamodel/instances/GuiObject.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_INSTANCE(GuiObject, Instance);
	GuiObject::GuiObject(std::string name) : Instance(name) {

	}

	void GuiObject::setVisible(const bool visible) {
		if (m_visible != visible) {
			this->changing.call("Visible", visible);
			m_visible = visible;
			this->changed.call("Visible");
		}
	}

	void GuiObject::setClipToBounds(const bool clipToBounds) {
		if (m_clipToBounds != clipToBounds) {
			this->changing.call("ClipToBounds", clipToBounds);
			m_clipToBounds = clipToBounds;
			this->changed.call("ClipToBounds");
		}
	}

	void GuiObject::properties(ClassDescriptor* desc) {
		desc->addProperty(new TypedProperty<GuiObject, bool>(
			"ClipToBounds",
			"Data",
			&GuiObject::clipsToBounds,
			&GuiObject::setClipToBounds
		));

		desc->addProperty(new TypedProperty<GuiObject, bool>(
			"Visible",
			"Data",
			&GuiObject::isVisible,
			&GuiObject::setVisible
		));
	}
}