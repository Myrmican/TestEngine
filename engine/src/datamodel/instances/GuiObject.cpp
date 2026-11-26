#include <datamodel/instances/GuiObject.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_INSTANCE(GuiObject, Instance);
	GuiObject::GuiObject(std::string name) : Instance(name) {

	}

	void GuiObject::setVisible(const bool visible) {
		if (m_visible != visible) {
			changing.call("Visible", visible);
			m_visible = visible;
			changed.call("Visible");
		}
	}

	void GuiObject::setClipsToBounds(const bool clipsToBounds) {
		if (m_clipsToBounds != clipsToBounds) {
			changing.call("ClipToBounds", clipsToBounds);
			m_clipsToBounds = clipsToBounds;
			changed.call("ClipToBounds");
		}
	}

	void GuiObject::setRotation(const float rotation) {
		if (m_rotation != rotation) {
			changing.call("Rotation", rotation);
			m_rotation = rotation;
			changed.call("Rotation");
		}
	}

	void GuiObject::properties(ClassDescriptor* desc) {
		desc->addProperty(new TypedProperty<GuiObject, bool>(
			"ClipsToBounds",
			"Data",
			&GuiObject::clipsToBounds,
			&GuiObject::setClipsToBounds
		));

		desc->addProperty(new TypedProperty<GuiObject, bool>(
			"Visible",
			"Data",
			&GuiObject::isVisible,
			&GuiObject::setVisible
		));

		desc->addProperty(new TypedProperty<GuiObject, float>(
			"Rotation",
			"Data",
			&GuiObject::getRotation,
			&GuiObject::setRotation
		));
	}
}