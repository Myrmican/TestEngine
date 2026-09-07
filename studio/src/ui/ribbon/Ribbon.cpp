#include <ui/menus/MenuManager.h>
#include <editor/tools/ToolManager.h>
#include <ui/ribbon/RibbonStats.h>
#include <QMenu>
#include <QFrame>
#include <QTabBar>
#include <QHBoxLayout>
#include <QLabel>
#include <QSizePolicy>
#include <QMainWindow>
#include <algorithm>
#include <vector>

std::vector<std::string> ribbonTabNames = { "Home", "Model", "Avatar", "Test", "Plugins" };

void ConnectContextMenu(QWidget* ribbonBar, QTabBar* ribbonTabs, QMainWindow* window, QWidget* statsWidget) {
    QObject::connect(ribbonBar, &QWidget::customContextMenuRequested,
        [ribbonBar, ribbonTabs, statsWidget](const QPoint& pos) {
            QPoint tabBarPos = ribbonBar->mapFrom(ribbonBar, pos);

            QMenu* contextMenu = Menu::create(ribbonBar);

            int index = ribbonTabs->tabAt(tabBarPos);

            if (index != -1) {
				QMenu* tabMenu = Menu::create(ribbonTabs);
                //tabMenu->addAction("Rename tab");
                tabMenu->addAction("Hide tab", [ribbonTabs, index]() {
                    ribbonTabs->setTabVisible(index, false);
                });

                QAction* selectedAction = tabMenu->exec(ribbonBar->mapToGlobal(pos));

            } else {
                QMenu* ribbonTabMenu = Menu::create(contextMenu, "Ribbon tabs");

                contextMenu->addMenu(ribbonTabMenu);

                QAction* manageRibbonTabs = ribbonTabMenu->addAction("Manage");
				QMenu* ribbonTabsList = Menu::create(ribbonTabMenu, "Toggle");
                ribbonTabMenu->addMenu(ribbonTabsList);

                QMenu* ribbonStats = Engine::Ribbon::createStatsToggleMenu(contextMenu, statsWidget);
                contextMenu->addMenu(ribbonStats);

                QList<QAction*> tabActions = ribbonTabs->findChildren<QAction*>();

                for (const auto& tabName : ribbonTabNames) {
                    QAction* ribbonTabAction = ribbonTabsList->addAction(QString::fromStdString(tabName));
                    ribbonTabAction->setCheckable(true);

                    for (int i = 0; i < ribbonTabs->count(); ++i) {
                        if (ribbonTabs->tabText(i) != QString::fromStdString(tabName)) continue;

                        ribbonTabAction->setChecked(ribbonTabs->isTabVisible(i));
                        QObject::connect(ribbonTabAction, &QAction::triggered, [ribbonTabs, i]() {
                            ribbonTabs->setTabVisible(i, !ribbonTabs->isTabVisible(i));
                        });
                        break;
                    }
                }

                QAction* selectedAction = contextMenu->exec(ribbonBar->mapToGlobal(pos));

                if (selectedAction == manageRibbonTabs) {
                    qDebug() << "Manage ribbon tabs action triggered";
                }
            }
        }
    );
}

QWidget* setupStats(QWidget* parent) {
    QWidget* statsContainer = new QWidget(parent);
    statsContainer->setObjectName("RibbonStats");

    QHBoxLayout* statsLayout = new QHBoxLayout(statsContainer);
    statsLayout->setContentsMargins(8, 8, 30, 8);
    statsLayout->setSpacing(16);
    statsLayout->setObjectName("RibbonStatsLayout");

    statsContainer->setStyleSheet(
        "#RibbonStats QLabel {"
        "    color: #cccccc;"
        "    font-size: 13px;"
        "}"
        "#RibbonStats QLabel[role=\"value\"] {"
        "    color: #ffffff;"
        "    font-weight: bold;"
        "}"
    );

    return statsContainer;
};

QWidget* setupQuickActions(QWidget* parent) {
	QWidget* actionsContainer = new QWidget(parent);
	actionsContainer->setObjectName("RibbonQuickActions");

    QHBoxLayout* actionsLayout = new QHBoxLayout(actionsContainer);
    actionsLayout->setContentsMargins(8, 8, 8, 8);
    actionsLayout->setSpacing(16);

    /*QWidget* playAction = ToolManager::createQuickTool("Play", actionsContainer);
	actionsLayout->addWidget(playAction);*/

    return actionsContainer;
}

namespace Engine {
    namespace Ribbon {
        QFrame* createWidget(QMainWindow* parent) {

            QFrame* ribbonBar = new QFrame(parent);
            ribbonBar->setObjectName("RibbonFrame");
            ribbonBar->setStyleSheet("QFrame { background-color: #1c1c1c; }");
            ribbonBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
            ribbonBar->setContextMenuPolicy(Qt::CustomContextMenu);
            ribbonBar->setFixedHeight(36);

            QTabBar* ribbonTabs = new QTabBar(ribbonBar);
            ribbonTabs->setObjectName("RibbonTabs");
            ribbonTabs->setDrawBase(false);
            ribbonTabs->setElideMode(Qt::ElideRight);
            ribbonTabs->setUsesScrollButtons(false);
            ribbonTabs->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
            ribbonTabs->setExpanding(false);
            ribbonTabs->setFixedHeight(32);

            ribbonTabs->setStyleSheet(
                "QTabBar {"
                "    background: transparent;"
                "    border: none;"
                "    outline: none;"
                "}"
                "QTabBar::tab {"
                "   background: transparent;"
                "   color: #9ca3af;"
                "   height: 32px;"
                "   min-width: 36px;"
                "   padding: 0 16px;"
                "   margin-right: 2px;"
                "   border-top-left-radius: 5px;"
                "   border-top-right-radius: 5px;"
                "   border-top: 2px solid transparent;"
                "}"
                "QTabBar::tab:hover {"
                "    background-color: #252525;"
                "    color: #cccccc;"
                "}"
                "QTabBar::tab:selected {"
                "    background-color: #161616;"
                "    color: #ffffff;"
                "    font-weight: 750;"
                "    border-top: 2px solid #3b82f6;"
                "}"
            );

            QWidget* statsWidget = setupStats(ribbonBar);
			QWidget* quickActions = setupQuickActions(ribbonBar);

            QHBoxLayout* layout = new QHBoxLayout(ribbonBar);
            layout->setContentsMargins(8, 4, 8, 0);
            layout->setSpacing(16);
            layout->addWidget(ribbonTabs);
			layout->addWidget(quickActions);
            layout->addStretch();
            layout->addWidget(statsWidget);

			for (const auto& tabName : ribbonTabNames) {
				auto ribbonTab = ribbonTabs->addTab(QString::fromStdString(tabName));
			}

			ConnectContextMenu(ribbonBar, ribbonTabs, parent, statsWidget);

            return ribbonBar;
        }
    }
}