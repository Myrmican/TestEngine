#pragma once

class QWidget;

namespace Engine {
	class EditorPlugin {
	public:
		EditorPlugin(QWidget* editorPage);
		~EditorPlugin();

		QWidget* getEditor() { return m_editorPage; }

	private:
		QWidget* m_editorPage;
	};
}