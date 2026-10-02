#pragma once

#include <datamodel/Instance.h>

namespace Engine {
	class Entity : public Creatable {
	public:
		Entity();

		double getHealth() const;

		void setHealth(double health);

		static void properties(ClassDescriptor* desc);

	private:
		double m_health = 100;
	};
}