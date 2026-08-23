#include <datamodel/Instance.h>

Instance::setParentInternal(Instance* newParent) {

	if (newParent == parent) {
		return true;
	}

	if (this == newParent) {
		std::string message = "Attempt to set %s as its own parent";
	}

	this->parent = newParent;
}