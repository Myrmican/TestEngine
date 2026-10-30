#include <datamodel/instances/BasePart.h>
#include <core/Reflection.h>

#include <string>

namespace Engine {
	REGISTER_INSTANCE(BasePart, Instance);
	BasePart::BasePart(std::string className) : Creatable(className),
		m_scale(1, 1, 1),
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
			m_scale.x, m_scale.y, m_scale.z);

		return scale * getCFrame();
	}

	void BasePart::setScale(Vector3 scale) {
		if (m_scale != scale)
			this->changed.call("Scale");
		m_scale = scale;
	}

	void BasePart::setPosition(Vector3 position) {
		if (m_position != position)
			this->changed.call("Position");
		m_position = position;
	}

	void BasePart::setRotation(Vector3 rotation) {
		if (m_rotation != rotation)
			this->changed.call("Rotation");
		m_rotation = rotation;
	}

	void BasePart::setColor(const Color3 color) {
		if (m_color != color)
			this->changed.call("Color");
		m_color = color;
	}

	void BasePart::setStatic(const bool makeStatic) {
		if (m_static != makeStatic)
			this->changed.call("Static");
		m_static = makeStatic;
	}

	void BasePart::setShadows(const bool shadows) {
		if (m_shadows != shadows)
			this->changing.call("Shadows", shadows);
		m_shadows = shadows;
	}

	void BasePart::setOpacity(const float opacity) {
		if (m_opacity != opacity)
			this->changing.call("Opacity", opacity);
		m_opacity = opacity;
	}

	void BasePart::setCollidable(const bool collidable) {
		if (m_collidable != collidable)
			this->changing.call("Collidable", collidable);
		m_collidable = collidable;
	}

	void BasePart::properties(Engine::ClassDescriptor* desc) {
		desc->addProperty(new TypedProperty<BasePart, Color3>(
			"Color",
			"Appearance",
			&BasePart::getColor,
			&BasePart::setColor
		));

		desc->addProperty(new TypedProperty<BasePart, bool>(
			"Shadows",
			"Appearance",
			&BasePart::hasShadows,
			&BasePart::setShadows
		));

		desc->addProperty(new TypedProperty<BasePart, float>(
			"Opacity",
			"Appearance",
			&BasePart::getOpacity,
			&BasePart::setOpacity
		));

		desc->addProperty(new TypedProperty<BasePart, bool>(
			"Static",
			"Part",
			&BasePart::isStatic,
			&BasePart::setStatic
		));

		desc->addProperty(new TypedProperty<BasePart, bool>(
			"Collidable",
			"Part",
			&BasePart::isCollidable,
			&BasePart::setCollidable
		));

		desc->addProperty(new TypedProperty<BasePart, Vector3>(
			"Position",
			"Transform",
			&BasePart::getPosition,
			&BasePart::setPosition
		));

		desc->addProperty(new TypedProperty<BasePart, Vector3>(
			"Rotation",
			"Transform",
			&BasePart::getRotation,
			&BasePart::setRotation
		));

		desc->addProperty(new TypedProperty<BasePart, Vector3>(
			"Scale",
			"Transform",
			&BasePart::getScale,
			&BasePart::setScale
		));
	}
}