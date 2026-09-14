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
		Vector3 size{ 2.0f, 1.0f, 4.0f };    // Default block dimensions
		Color3 color{ 0.8f, 0.8f, 0.8f };

		DirectX::XMMATRIX GetWorldMatrix() const {
			using namespace DirectX;
			XMMATRIX matScale = XMMatrixScaling(size.x, size.y, size.z);
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