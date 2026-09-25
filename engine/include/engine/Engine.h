#pragma once

#include <memory>
#include <datamodel/DataModel.h>
#include <datamodel/Service.h>

namespace Engine {
	class ServiceProvider;
	
	class EngineInstance {

	public:
		EngineInstance();
		~EngineInstance() = default;

		DataModel* getDataModel() { return m_dataModel.get(); }

		Engine::ServiceProvider* getProvider() { return m_sProvider; }
	private:
		std::unique_ptr<DataModel> m_dataModel;
		Engine::ServiceProvider* m_sProvider;
	};
}