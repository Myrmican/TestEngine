#include <datamodel/instances/Folder.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_CREATABLE(Folder, Instance);
	Folder::Folder(std::string_view name) : Creatable(name.data())
		,m_color(0, 0, 0) {
	}

	void Folder::properties(ClassDescriptor* desc) {
		desc->addProperty(new TypedProperty<Folder, Color3>(
			"Color",
			"Appearance",
			&Folder::getColor,
			&Folder::setColor
		));
	}

}