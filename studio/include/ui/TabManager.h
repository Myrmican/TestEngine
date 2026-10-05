#include <QTabWidget>
#include <QPoint>

class QToolBar;
class Project;
class QWidget;

namespace TabManager {
	class FileTabs : public QTabWidget {
		Q_OBJECT

	public:
		explicit FileTabs(QWidget* parent = nullptr);

		void handleTabClose(int index, int projectTabIndex, QWidget* editorPage,
			QMainWindow* window, QToolBar* mainToolBar, Project* project);

	private slots:
		void showTabContextMenu(const QPoint& pos);
	};
}