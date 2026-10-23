#pragma once

#include <memory>
#include <string>

namespace Engine {
	class Instance;

	class PlaceSerializer {
	public:
		virtual ~PlaceSerializer() = default;

		virtual bool saveToFile(const std::shared_ptr<Instance>& game, const std::string& filePath) = 0;

		virtual void loadFromFile(const std::string& filePath) = 0;

		virtual void loadRBXLX(std::string_view filePath) = 0;
	};
}