#include <services/selection/Selection.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_INTERNAL(Selection);
	Selection::Selection() : Instance("Selection") {
		internalLocked = true;
	}

	void Selection::select(Instance* instance) {
		selectionChanged.call(instance);
	}

	void Selection::deselect(Instance* instance) {
		selectionChanged.call(nullptr);
	}
}