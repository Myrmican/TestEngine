#include <datamodel/instances/OverlayBase.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_INSTANCE(OverlayBase, Instance);
	OverlayBase::OverlayBase(std::string_view name) : Creatable(name.data()) {

	}

	void OverlayBase::properties(ClassDescriptor* desc) {
		desc->addProperty(new TypedProperty<OverlayBase, bool>(
			"Enabled",
			"Data",
			&OverlayBase::isEnabled,
			&OverlayBase::setEnabled
		));
	}

}