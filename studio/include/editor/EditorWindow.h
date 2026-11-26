class QWidget;
class MainWindow;
class Project;

namespace Engine {
	class EditorWindow {
	public:
		EditorWindow(Project* project, QWidget* editorPageContainer, MainWindow* mainWindow);
	};
}