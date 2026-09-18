#include <datamodel/instances/BasePart.h>
#include <core/Reflection.h>

#include <string>

namespace Engine {
	REGISTER_INSTANCE(BasePart);
	BasePart::BasePart(std::string className) : Creatable(className),
		m_size(1, 1, 1),
		m_color(0.0f, 0.0f, 0.0f)
	{

	}

	void BasePart::setSize(Vector3 size) {
		if (m_size != size)
			this->changed.call("Size", m_size);
		m_size = size;
	}

	void BasePart::setPosition(Vector3 position) {
		if (m_position != position)
			this->changed.call("Size", m_position);
		m_position = position;
	}

	void BasePart::setColor(const Color3 color) {
		if (m_color != color)
			this->changed.call("Color", m_color);
		m_color = color;
	}

	void BasePart::reflectProperties(Engine::ClassDescriptor* desc) {
		Instance::reflectProperties(desc);

		desc->addProperty(new TypedProperty<BasePart, Vector3>(
			"Size",
			"Transform",
			&BasePart::getSize,
			&BasePart::setSize
		));

		desc->addProperty(new TypedProperty<BasePart, Color3>(
			"Color",
			"Data",
			&BasePart::getColor,
			&BasePart::setColor
		));
	}
}