#pragma once

#include <datamodel/Instance.h>
#include <core/math/Vector3.h>
#include <core/math/Color3.h>
#include <DirectXMath.h>
#include <string>

namespace Engine {
	class BasePart : public Creatable {
	public:
		DirectX::XMMATRIX getCFrame() const;

		DirectX::XMMATRIX getWorldMatrix() const;

		Vector3 getScale() const { return m_scale; }
		void setScale(Vector3 scale);

		Vector3 getPosition() const { return m_position; }
		void setPosition(Vector3 position);

		Vector3 getRotation() const { return m_rotation; }
		void setRotation(Vector3 rotation);

		Color3 getColor() const { return m_color; }
		void setColor(const Color3 color);

		bool isStatic() const { return m_static; }
		void setStatic(const bool makeStatic);

		bool isCollidable() const { return m_collidable; }
		void setCollidable(const bool collidable);

		bool hasShadows() const { return m_shadows; }
		void setShadows(const bool shadows);

		float getOpacity() const { return m_opacity; }
		void setOpacity(const float opacity);

		static void properties(ClassDescriptor* desc);
	protected:
		BasePart(std::string className);
	private:
		Vector3 m_scale;
		Vector3 m_position;
		Vector3 m_rotation;
		Color3 m_color;
		bool m_static = true;
		bool m_collidable = true;
		bool m_shadows = true;
		float m_opacity = 1;
	};
}