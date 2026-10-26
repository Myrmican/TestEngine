#include <datamodel/instances/Overlay.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_CREATABLE(Overlay, Instance);
	Overlay::Overlay(std::string_view name) : Creatable(name.data()) {

	}

	void Overlay::properties(ClassDescriptor* desc) {
		desc->addProperty(new TypedProperty<Overlay, bool>(
			"Enabled",
			"Data",
			&Overlay::isEnabled,
			&Overlay::setEnabled
		));
	}

}