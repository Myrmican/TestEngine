#include <datamodel/Property.h>
#include <datamodel/Instance.h>
#include <format>

namespace Engine {

	Property::Property(std::string name, std::string category) {
		m_name = name;
		m_category = category;
	}

    template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };

    std::string Property::getFormatted(const Instance* instance) const {
        PropertyValue rawValue = this->getValue(instance);

        return std::visit(overloaded{
            [](std::monostate) { return std::string(""); },
            [](std::string& s) { return s; },
            [](std::string_view sv) { return std::string(sv); },
            [](Engine::Instance* ptr) { return ptr ? std::string(ptr->getName()) : ""; },
            [](const Engine::Color3& color3) {
                return std::format("{}, {}, {}", color3.m_red * 255, color3.m_green * 255, color3.m_blue * 255);
            },
            [](const Engine::Vector3& vector3) {
                return std::format("{}, {}, {}", vector3.x, vector3.y, vector3.z);
            },
            [](float floatValue) { return std::format("{:.0f}", floatValue); },
            [](int intValue) { return std::format("{}", intValue); },
            [](bool b) { return b ? std::string("true") : std::string("false"); }
            }, rawValue);
    }
}