#include <ui/ribbon/RibbonStats.h>
#include <ui/menus/MenuManager.h>
#include <engine/core/Reflection.h>
#include <QMenu>
#include <QAction>

namespace Engine {
	namespace Ribbon {
		QMenu* createStatsToggleMenu(QWidget* parent) {
			QMenu* mainMenu = Menu::create(parent, "Stats");

			QMenu* instanceCount = Menu::create(parent, "Instance count");
			mainMenu->addMenu(instanceCount);

			for (const auto& className : GetCreateableClasses()) {
				QAction* instanceAction = instanceCount->addAction(QString::fromStdString(className));
				instanceAction->setCheckable(true);
			}

			return mainMenu;
		}
	}
}