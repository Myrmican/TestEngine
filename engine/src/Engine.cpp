#include <Engine.h>
#include <datamodel/Service.h>

namespace Engine {
	EngineInstance::EngineInstance() {
		m_sProvider = new ServiceProvider(this);
		m_dataModel = std::make_unique<DataModel>(this);
	}

    EngineInstance::~EngineInstance() {
        shutdown();
    }

    void EngineInstance::init() {
        if (m_isInitialized) return;

        // Initialize systems in strict dependency order
        // m_sProvider->init();

        m_isInitialized = true;
    }

    void EngineInstance::update(float dt) {
        if (!m_isInitialized) return;

        // Drive frame ticks, physics, scripting, and networking here
    }

    void EngineInstance::shutdown() {
        if (!m_isInitialized) return;

        // Graceful teardown of network loops and active tasks before destruction
        // m_sProvider->shutdown();

        m_isInitialized = false;
    }
}