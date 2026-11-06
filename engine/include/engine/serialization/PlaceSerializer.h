#pragma once

#include <memory>
#include <string>

namespace Engine {
	class Instance;
	class DataModel;

	class PlaceSerializer {
	public:
		virtual ~PlaceSerializer() = default;

		virtual bool saveToFile(const std::shared_ptr<Instance>& game, const std::string& filePath) = 0;

		static bool loadFromFile(const std::string& filePath, DataModel* dataModel);

		static bool loadRBXLX(std::string_view filePath, DataModel* dataModel);
	};
}