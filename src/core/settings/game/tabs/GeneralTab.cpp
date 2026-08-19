#include <core/settings/game/tabs/GeneralTab.h>
#include <QListWidget>
#include <QStackedWidget>
#include <QVBoxLayout>

GeneralTab::GeneralTab(QListWidget* listWidget, QStackedWidget* stackedWidget) {
	listWidget->addItem("General");

	auto* pageWidget = new QWidget(stackedWidget);
	auto* pageLayout = new QVBoxLayout(pageWidget);
	pageWidget->setLayout(pageLayout);

	stackedWidget->addWidget(pageWidget);
}