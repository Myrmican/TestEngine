#include <vector>
#include <string>

class QIcon;
class QString;
class QObject;
class QWidget;
class QAction;
class QToolBar;

namespace ToolManager {
	QAction* createToolAction(const QIcon& icon, const QString& text, QObject* parent);
	QWidget* createToolButtonWidget(QAction* action, QToolBar* toolBar, bool createLabel);
	QWidget* createQuickTool(const QString& objectName, QWidget* parent = nullptr);

    /*inline std::vector<std::string>& GetTool() {
        return GetReflectionRegistry().names;
    }*/
}

#define REGISTER_TOOL(toolName) \
    inline const ::int studioTool_##toolName( \
        #toolName, \
        []() -> std::unique_ptr<::Engine::Createable> { \
            return std::make_unique<toolName>(); \
        } \
    )