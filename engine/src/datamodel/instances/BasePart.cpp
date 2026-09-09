#include <datamodel/instances/BasePart.h>
#include <core/Reflection.h>

#include <string>

namespace Engine {
	REGISTER_CLASS(BasePart);
	BasePart::BasePart(std::string className) : Createable("BasePart"),
		m_size(1, 1, 1),
		m_color(0.0f, 0.0f, 0.0f)
	{

	}
}