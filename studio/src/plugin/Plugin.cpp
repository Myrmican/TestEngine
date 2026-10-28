#include <plugin/Plugin.h>
#include <QWidget>

namespace Engine {
	EditorPlugin::EditorPlugin(QWidget* editorPage) {
		m_editorPage = editorPage;
	}
}