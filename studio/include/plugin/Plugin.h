#pragma once

#include <ui/window/MainWindow.h>
#include <string>

class QWidget;

namespace Engine {
	class EditorPlugin {
	public:
		EditorPlugin(std::string_view pluginId, QWidget* editorPage);
		~EditorPlugin();

		std::string_view getName() const { return name; }
		MainWindow* getWindow() { return m_window; }

	private:
		std::string_view name;
		MainWindow* m_window;
	};
}