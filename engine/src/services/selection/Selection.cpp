#include <services/selection/Selection.h>
#include <iostream>

namespace Engine {
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