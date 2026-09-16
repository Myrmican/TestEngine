#include <datamodel/instances/Camera.h>
#include <core/Reflection.h>

using namespace Engine;
using namespace DirectX;

namespace Engine {
	REGISTER_CREATABLE(Camera);
	Camera::Camera() : Creatable("Camera") {}

	void Camera::setFOV(const float FOV) {
		if (m_FieldOfView != FOV)
			this->changed.call("FOV", m_FieldOfView);
		m_FieldOfView = FOV;
	}

	void Camera::setNearPlane(const float value) {
		if (m_NearPlane != value) {
			m_NearPlane = value;
			this->changed.call("NearPlane", m_NearPlane);
		}
	}

	void Camera::setFarPlane(const float value) {
		if (m_FarPlane != value) {
			m_FarPlane = value;
			this->changed.call("FarPlane", m_FarPlane);
		}
	}

	void Camera::setCFrame(const XMMATRIX& cframe) {
		m_CFrame = cframe;
		this->changed.call("CFrame", 0); // adjust to however your signal expects non-primitive payloads
	}

	XMMATRIX Camera::getViewMatrix() const {
		// The view matrix is just the INVERSE of the camera's world transform.
		// Intuition: moving the camera right is equivalent to moving the
		// entire world left. XMMatrixInverse does that flip for us.
		XMVECTOR det;
		return XMMatrixInverse(&det, m_CFrame);
	}

	XMMATRIX Camera::getProjectionMatrix(float aspectRatio) const {
		// Converts the 3D frustum (a pyramid-shaped volume in front of the
		// camera) into clip space. This is what gives you perspective -
		// distant objects appear smaller.
		float fovRadians = XMConvertToRadians(m_FieldOfView);
		return XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, m_NearPlane, m_FarPlane);
	}

	void Camera::reflectProperties(ClassDescriptor* desc) {
		Instance::reflectProperties(desc);

		desc->addProperty(new TypedProperty<Camera, float>(
			"FieldOfView",
			"Viewport",
			&Camera::getFOV,
			&Camera::setFOV
		));
	}
}