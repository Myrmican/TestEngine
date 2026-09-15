#pragma once

#include <datamodel/Instance.h>
#include <core/math/Vector3.h>
#include <core/math/Color3.h>
#include <DirectXMath.h>
#include <string>

namespace Engine {
	class BasePart : public Creatable {
	public:
		Vector3 position{ 0.0f, 0.0f, 0.0f };
		Vector3 rotation{ 0.0f, 0.0f, 0.0f }; // Yaw, Pitch, Roll in degrees

		Vector3 getSize() const { return m_size; }

		void setSize(Vector3 size) {
			if (m_size != size)
				this->changed.call("Size", m_size);
			m_size = size;
		}

		Color3 getColor() const { return m_color; }

		void setColor(const Color3 color) {
			if (m_color != color)
				this->changed.call("Color", m_color);
			m_color = color;
		}

		DirectX::XMMATRIX GetWorldMatrix() const {
			using namespace DirectX;
			XMMATRIX matScale = XMMatrixScaling(m_size.x, m_size.y, m_size.z);
			XMMATRIX matRot = XMMatrixRotationRollPitchYaw(
				XMConvertToRadians(rotation.x),
				XMConvertToRadians(rotation.y),
				XMConvertToRadians(rotation.z)
			);
			XMMATRIX matTrans = XMMatrixTranslation(position.x, position.y, position.z);

			return matScale * matRot * matTrans;
		}

		static void reflectProperties(ClassDescriptor* desc);
	protected:
		BasePart(std::string className);
	private:
		Vector3 m_size;
		Color3 m_color;
	};
}