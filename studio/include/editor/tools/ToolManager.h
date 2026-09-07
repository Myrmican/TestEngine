#include <vector>
#include <string>
#include <QIcon>

class QString;
class QObject;
class QWidget;
class QAction;
class QToolBar;
class QToolButton;

namespace ToolManager {
    QToolButton* createTool(QIcon icon, QString name, QToolBar* toolBar, bool createLabel);
    QToolButton* createQuickTool(const QString& objectName, QWidget* parent = nullptr);
}

#define REGISTER_TOOL(toolName) \
    inline const ::int studioTool_##toolName( \
        #toolName, \
        []() -> std::unique_ptr<::Engine::Createable> { \
            return std::make_unique<toolName>(); \
        } \
    )