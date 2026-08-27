#pragma once

#include <datamodel/Instance.h>

#include <map>
#include <string>

namespace Engine {
	class Game : public Instance {
	public:
		std::map<std::string, std::shared_ptr<Instance>> m_services;

		Game();
		~Game() override = default;

		
	};
}