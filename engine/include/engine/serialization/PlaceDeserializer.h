#pragma once

#include <datamodel/Instance.h>
#include <memory>
#include <string>

namespace Engine {
	class Instance;
	class DataModel;
	class EngineInstance;

	class PlaceDeserializer {
	public:
		virtual ~PlaceDeserializer() = default;

		static bool loadFromFile(std::string_view placeData, Engine::EngineInstance* engine);

		static bool loadFromRoblox(std::string_view filePath, std::string_view templateData, DataModel* dataModel);

		static bool loadScene(std::string_view sceneName, std::string_view filePath, InstancePtr dataModel);
	};
}