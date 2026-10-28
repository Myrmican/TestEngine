#include <plugin/classes/PluginDockWidget.h>
#include <plugin/Plugin.h>
#include <QDockWidget>
#include <QVBoxLayout>
#include <QTimer>
#include <qstring.h>

namespace EditorPlugin {
	REGISTER_INSTANCE(PluginDockWidget, Engine::Instance);
	PluginDockWidget::PluginDockWidget(Engine::EditorPlugin* plugin, std::string_view title) : GuiBase("PluginDockWidget") {
		//Check if a plugin is calling it / the environment is studio

		QString titleString = QString::fromUtf8(title.data());
		QDockWidget* dockWidget = new QDockWidget(titleString, plugin->getWindow());
		dockWidget->setWindowFlags(Qt::SubWindow);

		auto* containerWidget = new QWidget(dockWidget);
		auto* layout = new QVBoxLayout(containerWidget);
		layout->setContentsMargins(4, 4, 4, 4);
		layout->setSpacing(4);

		QTimer::singleShot(0, plugin->getWindow(), [plugin, dockWidget]() {
			plugin->getWindow()->resizeDocks({dockWidget}, {400}, Qt::Horizontal);
			});
	}
}