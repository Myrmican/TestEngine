#include <QTabWidget>
#include <QPoint>

class QToolBar;
class Project;
class QWidget;
class EngineViewport;

namespace TabManager {
	class FileTabs : public QTabWidget {
		Q_OBJECT

	public:
		explicit FileTabs(QWidget* parent = nullptr);

		void handleTabClose(int index, EngineViewport* placeView, QWidget* editorPage,
			QMainWindow* window, Project* project);

	private slots:
		void showTabContextMenu(const QPoint& pos);
	};
}