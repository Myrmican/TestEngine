#pragma once

#include <memory>
#include <string>

namespace Engine {
	class Instance;
	class DataModel;

	class PlaceSerializer {
	public:
		virtual ~PlaceSerializer() = default;

		static bool saveToFile(const DataModel* dataModel, const std::string_view filePath);
	};
}