#include <datamodel/Service.h>
#include <datamodel/Instance.h>
#include <Engine.h>

namespace Engine {
	ServiceProvider::ServiceProvider(EngineInstance* engine) {
		m_engine = engine;
	}

	void ServiceProvider::create(Instance* service) {
		if (service) {
			m_services.emplace_back(service->getClassName(), service);
		}
	}

	Instance* ServiceProvider::getService(std::string_view name) {
		for (const auto& [serviceName, instance] : m_services) {
			if (serviceName == name) {
				return instance;
			}
		}
		return nullptr;
	}
}