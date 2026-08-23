class QTabWidget;
class QToolBar;
class Project;
class QSplitter;

namespace TabManager {
	QTabWidget* createWidget(QSplitter* parent);

	void handleTabClose(int index, int projectTabIndex, QTabWidget* documentTabs, QWidget* editorPage,
		QMainWindow* window, QToolBar* mainToolBar, Project* project);
}