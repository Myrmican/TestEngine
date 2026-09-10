#include <datamodel/instances/Camera.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_CREATEABLE(Camera);
	Camera::Camera() : Createable("Camera") {

	}

	DirectX::XMMATRIX Camera::GetViewMatrix() const {
		using namespace DirectX;

		XMVECTOR pos = XMLoadFloat3(&position);

		// Calculate forward look vector from Euler rotation angles
		float pitch = XMConvertToRadians(rotation.x);
		float yaw = XMConvertToRadians(rotation.y);
		float roll = XMConvertToRadians(rotation.z);

		XMMATRIX rotMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
		XMVECTOR defaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f); // LH default forward (+Z)
		XMVECTOR forward = XMVector3TransformCoord(defaultForward, rotMatrix);

		XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		return XMMatrixLookToLH(pos, forward, up);
	}

	DirectX::XMMATRIX Camera::GetProjectionMatrix(float aspectRatio) const {
		using namespace DirectX;
		float fovRad = XMConvertToRadians(FOV);
		return XMMatrixPerspectiveFovLH(fovRad, aspectRatio, nearPlane, farPlane);
	}

	DirectX::XMMATRIX Camera::GetViewProjectionMatrix(float aspectRatio) const {
		return GetViewMatrix() * GetProjectionMatrix(aspectRatio);
	}
}