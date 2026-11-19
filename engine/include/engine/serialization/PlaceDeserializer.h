#pragma once

#include <memory>
#include <string>

namespace Engine {
	class Instance;
	class DataModel;

	class PlaceDeserializer {
	public:
		virtual ~PlaceDeserializer() = default;

		static bool loadFromFile(std::string_view placeData, DataModel* dataModel);

		static bool loadRBXLX(std::string_view filePath, DataModel* dataModel);
	};
}