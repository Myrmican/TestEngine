#pragma once

#include <datamodel/Instance.h>

#include <map>
#include <string>
#include <memory>

namespace Engine {
	class Game : public Instance {
	public:
		std::vector<std::pair<std::string, std::shared_ptr<Instance>>> m_services;

		Game();
		~Game() override = default;

		
	};
}