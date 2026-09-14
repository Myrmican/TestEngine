#pragma once

#include <datamodel/Instance.h>
#include <DirectXMath.h>

namespace Engine {
	class Camera : public Creatable {
	public:
		Camera();

		float getFOV() const { return m_FieldOfView; }

		void setFOV(const float FOV) {
			if (m_FieldOfView != FOV)
				this->changed.call("FOV", m_FieldOfView);
			m_FieldOfView = FOV;
		}

		static void reflectProperties(ClassDescriptor* desc);

		float nearPlane = 0.1f;
		float farPlane = 1000.0f;

		DirectX::XMFLOAT3 position = { 0.0f, 2.0f, -10.0f };
		DirectX::XMFLOAT3 rotation = { 0.0f, 0.0f, 0.0f };

		DirectX::XMMATRIX GetViewMatrix() const;
		DirectX::XMMATRIX GetProjectionMatrix(float aspectRatio) const;
		DirectX::XMMATRIX GetViewProjectionMatrix(float aspectRatio) const;

	private:
		float m_FieldOfView;
	};
}