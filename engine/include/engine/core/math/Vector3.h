#pragma once

namespace Engine {
	class Vector3 {
	public:
		float x, y, z;

		Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
		Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

		bool operator==(const Vector3& other) const {
			return this->x == other.x &&
				this->y == other.y &&
				this->z == other.z;
		}

		bool operator!=(const Vector3& other) const {
			return !(*this == other);
		}
	};
}