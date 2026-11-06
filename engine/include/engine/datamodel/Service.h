#pragma once

#include <memory>
#include <string>
#include <map>
#include <vector>

namespace Engine {
	class Instance;
	class EngineInstance;
	
	class ServiceProvider {
	public:
		std::vector<std::pair<std::string, Instance*>> m_services;

		ServiceProvider(EngineInstance* engine);
		
		void create(Instance* service);

		Instance* getService(std::string_view name);

		template <typename T>
		T* getService() {
			for (const auto& [serviceName, instance] : m_services) {
				if (auto typedInstance = dynamic_cast<T*>(instance)) {
					return typedInstance;
				}
			}
			return nullptr;
		}
	private:
		EngineInstance* m_engine;
	};
}