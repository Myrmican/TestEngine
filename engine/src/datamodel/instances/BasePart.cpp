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

	DirectX::XMMATRIX BasePart::getCFrame() const
	{
		DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationRollPitchYaw(
			DirectX::XMConvertToRadians(m_rotation.x),
			DirectX::XMConvertToRadians(m_rotation.y),
			DirectX::XMConvertToRadians(m_rotation.z)
		);
		DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(m_position.x, m_position.y, m_position.z);

		// rotate first, then move into place - same reasoning as before:
		// translating first would orbit the part around the world origin
		// instead of spinning it in place.
		return rotation * translation;
	}

	DirectX::XMMATRIX BasePart::getWorldMatrix() const
	{
		DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(m_size.x, m_size.y, m_size.z);
		return scale * getCFrame(); // reuse it, rather than duplicate rotation/translation
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