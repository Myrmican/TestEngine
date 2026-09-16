#pragma once

#include <datamodel/Instance.h>
#include <DirectXMath.h>

namespace Engine {
	class Camera : public Creatable {
	public:
		Camera();

		float getFOV() const { return m_FieldOfView; }
		void setFOV(const float FOV);

		float getNearPlane() const { return m_NearPlane; }
		void setNearPlane(const float value);

		float getFarPlane() const { return m_FarPlane; }
		void setFarPlane(const float value);

		// Position + orientation, stored as a transform matrix (like Roblox's CFrame)
		DirectX::XMMATRIX getCFrame() const { return m_CFrame; }
		void setCFrame(const DirectX::XMMATRIX& cframe);

		// The two matrices your renderer actually needs each frame
		DirectX::XMMATRIX getViewMatrix() const;
		DirectX::XMMATRIX getProjectionMatrix(float aspectRatio) const;

		static void reflectProperties(ClassDescriptor* desc);

	private:
		float m_FieldOfView = 70.0f;   // degrees - Roblox default is 70
		float m_NearPlane = 0.1f;      // anything closer than this is clipped
		float m_FarPlane = 1000.0f;    // anything farther than this is clipped
		DirectX::XMMATRIX m_CFrame = DirectX::XMMatrixIdentity();
	};
}