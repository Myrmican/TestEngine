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

		Vector3 getSize() const { return m_size; }

		void setSize(Vector3 size);

		Vector3 getPosition() const { return m_position; }

		void setPosition(Vector3 position);

		Color3 getColor() const { return m_color; }

		void setColor(const Color3 color);

		static void reflectProperties(ClassDescriptor* desc);
	protected:
		BasePart(std::string className);
	private:
		Vector3 m_size;
		Vector3 m_position;
		Vector3 m_rotation;
		Color3 m_color;
	};
}