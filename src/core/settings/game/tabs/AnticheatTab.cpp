#include <core/settings/game/tabs/AnticheatTab.h>
#include <QListWidget>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMenu>
#include <QLabel>
#include <QToolButton>

AnticheatTab::AnticheatTab(QListWidget* listWidget, QStackedWidget* stackedWidget) {
	listWidget->addItem("Anticheat");

    auto* pageWidget = new QWidget(stackedWidget);
    auto* pageLayout = new QHBoxLayout(pageWidget);
    pageLayout->setContentsMargins(0, 0, 0, 0);

    pageWidget->setLayout(pageLayout);

    stackedWidget->addWidget(pageWidget);
}