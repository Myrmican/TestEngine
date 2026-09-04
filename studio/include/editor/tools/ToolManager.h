class QIcon;
class QString;
class QObject;
class QWidget;
class QAction;
class QToolBar;

namespace ToolManager {
	QAction* createToolAction(const QIcon& icon, const QString& text, QObject* parent);
	QWidget* createToolButtonWidget(QAction* action, QToolBar* toolBar);
	QWidget* getTool(const QString& objectName, QWidget* parent = nullptr);
}