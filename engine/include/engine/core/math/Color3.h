#pragma once

#include <string>
#include <unordered_map>

static const std::unordered_map<std::string_view, std::string_view> NamedColors = {
	{"White",  "#FFFFFF"},
	{"Orange", "#FFA500"},
	{"Red",    "#FF0000"},
	{"Blue",   "#0000FF"}
};

namespace Engine {
	class Color3 {
	public:
		float m_red = 0.0f;
		float m_green = 0.0f;
		float m_blue = 0.0f;

		Color3() = default;
		Color3(float red, float green, float blue);
		Color3(std::string_view color);

		bool operator==(const Color3& other) const {
			return m_red == other.m_red &&
				m_green == other.m_green &&
				m_blue == other.m_blue;
		}

		bool operator!=(const Color3& other) const {
			return !(*this == other);
		}

		std::string toHex();

		static Color3 fromRGB(float red, float green, float blue);
		static Color3 fromHex(std::string hex);
	};
}