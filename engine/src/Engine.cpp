#include <Engine.h>
#include <datamodel/Service.h>
#include <services/logging/Logging.h>
#include <format>

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

        m_isInitialized = true;

    }

    void EngineInstance::update(float dt) {
        if (!m_isInitialized) return;


    }

    void EngineInstance::shutdown() {
        if (!m_isInitialized) return;

        m_isInitialized = false;
    }
}