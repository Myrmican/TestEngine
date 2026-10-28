#include <plugin/classes/DockWidget.h>
#include <plugin/Plugin.h>
#include <QDockWidget>

namespace EditorPlugin {
	DockWidget::DockWidget(Engine::EditorPlugin* plugin, std::string_view title) {
		//Check if a plugin is calling it / the environment is studio

		QDockWidget* pluginWidget = new QDockWidget(title, plugin->getEditor());

		
	}
}