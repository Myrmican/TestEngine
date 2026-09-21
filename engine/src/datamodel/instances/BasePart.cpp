#include <datamodel/instances/BasePart.h>
#include <core/Reflection.h>

#include <string>

namespace Engine {
	REGISTER_INSTANCE(BasePart);
	BasePart::BasePart(std::string className) : Creatable(className),
		m_size(1, 1, 1),
		m_position(0, 0, 0),
		m_rotation(0, 0, 0),
		m_color(0.639f, 0.635f, 0.647f)
	{

	}

	DirectX::XMMATRIX BasePart::getCFrame() const
	{
		// ASSUMPTION: m_rotation is Euler angles in RADIANS, ordered
		// (pitch=x, yaw=y, roll=z) — XMMatrixRotationRollPitchYaw's
		// expected order. If your engine stores rotation as a
		// quaternion, a 3x3 basis, or degrees, this is the one line to
		// change (e.g. DirectX::XMMatrixRotationQuaternion(...), or
		// multiply by DirectX::XM_PI / 180.0f first for degrees).
		DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationRollPitchYaw(
			m_rotation.x, m_rotation.y, m_rotation.z);

		// Rotate around the part's own origin first, then move the
		// result out to its position in world space.
		DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(
			m_position.x, m_position.y, m_position.z);

		return rotation * translation;
	}

	DirectX::XMMATRIX BasePart::getWorldMatrix() const
	{
		// Scale happens in the part's own local space, BEFORE rotation
		// — do it the other way around and a non-uniform Size (e.g. a
		// long thin part) would get its axes mixed up by the rotation
		// instead of scaling along its own local axes correctly.
		DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(
			m_size.x, m_size.y, m_size.z);

		return scale * getCFrame();
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