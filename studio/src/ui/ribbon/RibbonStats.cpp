#include <ui/ribbon/RibbonStats.h>
#include <ui/menus/MenuManager.h>
#include <engine/core/Reflection.h>
#include <project/Project.h>
#include <QHBoxLayout>
#include <QMenu>
#include <QAction>
#include <QLabel>
#include <QObject>
#include <QString>
#include <string>

using namespace Engine::Ribbon;

GetStatistic Engine::Ribbon::getStatistic(std::string statName, QWidget* statsWidget) {
	QString statNameString = QString::fromStdString(statName);
	QStringList statNameSplit = statNameString.split("_");

	QWidget* top = statsWidget->window()->findChild<QWidget*>("ProjectEditorTopWidget");

	Project* currentProject = ProjectManager::getProject(top);
	if (!currentProject) return { QString::fromStdString(statName), "ERR" };

	if (statNameSplit[0] == "InstanceCount") {
		auto dataModel = currentProject->dataModel.get();

		auto descendants = dataModel->getDescendants();

		int result = 0;

		for (const InstancePtr& descendant : descendants) {
			if (descendant.get()->getClassName() != statNameSplit[1]) continue;

			result++;
		}

		return { statNameSplit[1] + " count", QString::fromStdString(std::to_string(result)) };
	}

	return { QString::fromStdString(statName), "0"};
}

static QLabel* Engine::Ribbon::statisticLabel(QWidget* parent, std::string statName) {

	QLabel* existingLabel = parent->findChild<QLabel*>(statName);
	if (existingLabel) return existingLabel;

	GetStatistic statistic = getStatistic(statName, parent);

	QLabel* statLabel = new QLabel(
		statistic.statName + ": <span style='color: #ffffff; font-size: 11px; font-weight: DemiBold;'>" +
		statistic.statValue + "</span>",
		parent
	);

	QFont interFont("Inter");
	statLabel->setFont(interFont);
	statLabel->setObjectName(statName);

	return statLabel;
}

QMenu* Engine::Ribbon::createStatsToggleMenu(QWidget* parent, QWidget* statsWidget) {
	QMenu* mainMenu = Menu::create(parent, "Stats");

	QMenu* instanceCount = Menu::create(parent, "Instance count");
	mainMenu->addMenu(instanceCount);

	QHBoxLayout* statsLayout = statsWidget->findChild<QHBoxLayout*>("RibbonStatsLayout");

	for (const auto& className : GetCreateableClasses()) {
		QAction* instanceAction = instanceCount->addAction(QString::fromStdString(className));
		instanceAction->setCheckable(true);

		QObject::connect(instanceAction, &QAction::toggled,
			[className, parent, statsWidget, statsLayout](bool checked) {
				if (!checked) return;

				QLabel* statLabel = statisticLabel(statsWidget, "InstanceCount_" + className);
				statsLayout->addWidget(statLabel);

				for (int i = 0; i < statsLayout->count(); i++) {
					QLayoutItem* item = statsLayout->itemAt(i);
					QWidget* w = item->widget();
				}

			});
	}

	return mainMenu;
}