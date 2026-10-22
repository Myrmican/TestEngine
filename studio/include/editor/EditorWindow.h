class QWidget;
class QMainWindow;
class Project;

namespace Engine {
	class EditorWindow {
	public:
		EditorWindow(Project* project, QWidget* editorPageContainer, QMainWindow* mainWindow);
	};
}