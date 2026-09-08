#pragma once

#include <datamodel/Instance.h>
#include <core/math/Vector3.h>
#include <core/math/Color3.h>
#include <string>

namespace Engine {
	class BasePart : public Createable {
	public:
		BasePart(std::string className);

	private:
		Vector3 m_size;
		Color3 m_color;
	};
}