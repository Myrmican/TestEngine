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