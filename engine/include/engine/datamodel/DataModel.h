#pragma once

#include <datamodel/Instance.h>

#include <map>
#include <string>
#include <memory>

namespace Engine {
	class EngineInstance;

	class DataModel : public Instance {
	public:
		DataModel(EngineInstance* engine);
		~DataModel() override = default;

	private:
		Engine::EngineInstance* m_engine;
	};
}