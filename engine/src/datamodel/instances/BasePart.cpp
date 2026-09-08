#include <datamodel/instances/BasePart.h>
#include <core/Reflection.h>

#include <string>

namespace Engine {
	BasePart::BasePart(std::string className) : Createable("BasePart"),
		m_size(0.0f, 0.0f, 0.0f),
		m_color(0.0f, 0.0f, 0.0f)
	{

	}
}