#pragma once

#include <string>

namespace Engine {
	class EditorPlugin;
}

namespace EditorPlugin {
	class DockWidget {
	public:
		DockWidget(Engine::EditorPlugin* plugin, std::string_view title);
		~DockWidget();
	};
}