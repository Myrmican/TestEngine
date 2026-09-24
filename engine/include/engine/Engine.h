#pragma once

#include <memory>
#include <datamodel/Game.h>

namespace Engine {
	class Engine {

	public:
		Engine() {
			m_dataModel = std::make_unique<Game>();
		}

		~Engine() = default;

	private:
		std::unique_ptr<Game> m_dataModel;
	};
}