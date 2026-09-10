#include <datamodel/instances/Folder.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_CREATEABLE(Folder);
	Folder::Folder() : Createable("Folder")
		,m_color(0, 0, 0) {
	}

	void Folder::reflectProperties(ClassDescriptor* desc) {
		Instance::reflectProperties(desc);

		desc->addProperty(new TypedProperty<Folder, Color3>(
			"Color",
			"Appearance",
			&Folder::getColor,
			&Folder::setColor
		));
	}

}