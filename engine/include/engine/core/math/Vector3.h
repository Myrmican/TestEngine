#pragma once

namespace Engine {
	class Vector3 {
	public:
		float x, y, z;

		Vector3(float x, float y, float z);

		bool operator==(const Vector3& other) const {
			return this->x == other.x &&
				this->z == other.y &&
				this->x == other.z;
		}

		bool operator!=(const Vector3& other) const {
			return !(*this == other);
		}
	};
}