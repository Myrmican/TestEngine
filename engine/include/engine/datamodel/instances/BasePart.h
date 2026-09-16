#pragma once

#include <datamodel/Instance.h>
#include <core/math/Vector3.h>
#include <core/math/Color3.h>
#include <DirectXMath.h>
#include <string>

namespace Engine {
	class BasePart : public Creatable {
	public:
		Vector3 getSize() const { return m_size; }

		void setSize(Vector3 size) {
			if (m_size != size)
				this->changed.call("Size", m_size);
			m_size = size;
		}

		Vector3 getPosition() const { return m_position; }

		void setPosition(Vector3 position) {
			if (m_position != position)
				this->changed.call("Size", m_position);
			m_position = position;
		}

		DirectX::XMMATRIX getCFrame() const;
		DirectX::XMMATRIX getWorldMatrix() const;

		Color3 getColor() const { return m_color; }

		void setColor(const Color3 color) {
			if (m_color != color)
				this->changed.call("Color", m_color);
			m_color = color;
		}

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