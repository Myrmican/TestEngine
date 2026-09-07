#include <editor/tools/ToolManager.h>
#include <QLabel>
#include <QToolButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QAction>
#include <QToolBar>
#include <QObject>
#include <QString>
#include <QIcon>
#include <QApplication>

namespace ToolManager {
    QToolButton* createTool(QIcon icon, QString name, QToolBar* toolBar, bool createLabel) {

        QToolButton* toolButton = new QToolButton(toolBar);

        toolButton->setFixedSize(64, 64);

        toolButton->setIcon(icon);
        toolButton->setIconSize(QSize(18, 18));

        toolButton->setFont(QFont("Inter", 10, 500));

        toolButton->setText(name);
        toolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        toolButton->setFocusPolicy(Qt::NoFocus);
        toolButton->setCheckable(true);

        return toolButton;
    }

    QToolButton* createQuickTool(const QString& objectName, QWidget* parent) {
        QToolButton* tool = createTool(QIcon(), "Test", nullptr, false);
        tool->setIconSize(QSize(8, 8));
        tool->setFixedSize(22, 22);

		return tool;
	}
}