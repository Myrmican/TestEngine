#pragma once

#include <memory>
#include <string>

namespace Engine {
	class Game;

	class PlaceSerializer {
	public:
		virtual ~PlaceSerializer() = default;

		virtual bool saveToFile(const std::shared_ptr<Game>& game, const std::string& filePath) = 0;

		virtual std::shared_ptr<Game> loadFromFile(const std::string& filePath) = 0;
	};
}