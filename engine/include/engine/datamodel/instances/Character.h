#pragma once

#include <datamodel/instances/Tool.h>
#include <datamodel/Instance.h>

namespace Engine {
	class Character : public Creatable {
	public:
		Character();

		double getHealth() const;

		void setHealth(double health);

		void equipTool(Tool* tool, bool leftHand = false);
		void unequipTools();

		static void properties(ClassDescriptor* desc);

	private:
		double m_health = 100;
	};
}