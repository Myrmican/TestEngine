#pragma once

#include <datamodel/Instance.h>
#include <core/Event.h>

namespace Engine {
	class Selection : public Instance {
	public:
		Event<Instance*> selectionChanged;

		Selection();

		void select(Instance* instance);
		void deselect(Instance* instance);
	};
}