#pragma once

#include <memory>
#include <datamodel/DataModel.h>
#include <datamodel/Service.h>

namespace Engine {
	class ServiceProvider;
	
	class EngineInstance {

	public:
		EngineInstance();
		~EngineInstance();

		void init();
		void update(float dt);
		void shutdown();

		DataModel* getDataModel() { return m_dataModel.get(); }
		Engine::ServiceProvider* getProvider() { return m_sProvider; }
	private:
		std::unique_ptr<DataModel> m_dataModel;
		ServiceProvider* m_sProvider;

		bool m_isInitialized = false;
	};
}