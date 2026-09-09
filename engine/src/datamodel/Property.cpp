#include <datamodel/Property.h>

namespace Engine {
	Property::Property(std::string name, std::string category) {
		m_name = name;
		m_category = category;
	}
}