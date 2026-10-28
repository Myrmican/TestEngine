#pragma once

#include <string>
#include <engine/datamodel/instances/GuiBase.h>
#include <engine/core/Reflection.h>

namespace Engine {
	class EditorPlugin;
}

namespace EditorPlugin {
	class PluginDockWidget : public Engine::GuiBase {
	public:
		PluginDockWidget(Engine::EditorPlugin* plugin, std::string_view title);
		~PluginDockWidget() = default;
	};
}