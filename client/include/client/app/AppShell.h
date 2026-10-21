#pragma once

#include <memory>

namespace Engine {
	class DataModel;
}

namespace Client {
	class AppShell {
	public:
		explicit AppShell(Engine::DataModel* dataModel);
		~AppShell() = default;

		void initialize();

		Engine::DataModel* m_dataModel = nullptr;
	};
}