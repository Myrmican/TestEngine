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
		float m_red;
		float m_green;
		float m_blue;

		Color3(float red, float green, float blue);
		Color3(std::string_view color);

		Color3 fromRGB(float red, float green, float blue);
		Color3 fromHex(std::string hex);

		
	};
}