#pragma once

#include <datamodel/Instance.h>
#include <DirectXMath.h>

namespace Engine {
	class Camera : public Createable {
	public:
		Camera();

		float FOV = 70.0f;
		float nearPlane = 0.1f;
		float farPlane = 1000.0f;

		DirectX::XMFLOAT3 position = { 0.0f, 2.0f, -10.0f };
		DirectX::XMFLOAT3 rotation = { 0.0f, 0.0f, 0.0f };

		DirectX::XMMATRIX GetViewMatrix() const;
		DirectX::XMMATRIX GetProjectionMatrix(float aspectRatio) const;
		DirectX::XMMATRIX GetViewProjectionMatrix(float aspectRatio) const;
	};
}