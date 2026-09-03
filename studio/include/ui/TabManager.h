class QTabWidget;
class QToolBar;
class Project;
class QWidget;

namespace TabManager {
	QTabWidget* createWidget(QWidget* parent);

	void handleTabClose(int index, int projectTabIndex, QTabWidget* documentTabs, QWidget* editorPage,
		QMainWindow* window, QToolBar* mainToolBar, Project* project);
}