#pragma once

#include <QMainWindow>
#include <string>

class QWidget;

namespace Engine {
	class EditorPlugin {
	public:
		EditorPlugin(std::string_view pluginId, QWidget* editorPage);
		~EditorPlugin();

		std::string_view getName() const { return name; }
		QMainWindow* getWindow() { return m_window; }

	private:
		std::string_view name;
		QMainWindow* m_window;
	};
}