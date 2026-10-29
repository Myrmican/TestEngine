#include <plugin/Plugin.h>
#include <QWidget>

namespace Engine {
	EditorPlugin::EditorPlugin(std::string_view pluginId, QWidget* editorPage) {
		m_window = qobject_cast<QMainWindow*>(editorPage->window());

		//API call from the plugin id to set the name of the plugin
		name = "Test plugin";
	}
}