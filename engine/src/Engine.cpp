#include <Engine.h>
#include <datamodel/Service.h>

namespace Engine {
	EngineInstance::EngineInstance() {
		m_sProvider = new ServiceProvider(this);
		m_dataModel = std::make_unique<DataModel>(this);
	}
}