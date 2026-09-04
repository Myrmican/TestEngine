#include <ui/menus/MenuManager.h>
#include <editor/tools/ToolManager.h>
#include <QMenu>
#include <QFrame>
#include <QTabBar>
#include <QHBoxLayout>
#include <QLabel>
#include <QSizePolicy>
#include <QMainWindow>
#include <algorithm>
#include <vector>

std::vector<std::string> ribbonTabNames = { "Home", "Model", "Avatar" };

void ConnectContextMenu(QWidget* ribbonBar, QTabBar* ribbonTabs, QMainWindow* window) {
    QObject::connect(ribbonBar, &QWidget::customContextMenuRequested,
        [ribbonBar, ribbonTabs, window](const QPoint& pos) {
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
    statsLayout->setContentsMargins(0, 0, 12, 0);
    statsLayout->setSpacing(16);

    statsContainer->setStyleSheet(
        "#RibbonStats QLabel {"
        "    color: #cccccc;"
        "    font-size: 13px;"
        "    padding: 8px 0px;"
        "}"
        "#RibbonStats QLabel[role=\"value\"] {"
        "    color: #ffffff;"
        "    font-weight: bold;"
        "}"
    );

    QLabel* stat1 = new QLabel("Triangles: <span style='color: #ffffff;'>12,321</span>", statsContainer);
    QLabel* stat2 = new QLabel("Parts: <span style='color: #ffffff;'>1,500</span>", statsContainer);

    statsLayout->addWidget(stat1);
    statsLayout->addWidget(stat2);

    return statsContainer;
};

QWidget* setupQuickActions(QWidget* parent) {
	QWidget* actionsContainer = new QWidget(parent);
	actionsContainer->setObjectName("RibbonQuickActions");

    QHBoxLayout* actionsLayout = new QHBoxLayout(actionsContainer);
    actionsLayout->setContentsMargins(0, 0, 12, 0);
    actionsLayout->setSpacing(16);

    //QWidget* playAction = ToolManager::getTool("Play", actionsContainer);
	//actionsLayout->addWidget(playAction);

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

            QTabBar* ribbonTabs = new QTabBar(ribbonBar);
            ribbonTabs->setObjectName("RibbonTabs");
            ribbonTabs->setDrawBase(false);
            ribbonTabs->setElideMode(Qt::ElideRight);
            ribbonTabs->setUsesScrollButtons(false);
            ribbonTabs->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
            ribbonTabs->setExpanding(false);

            ribbonTabs->setStyleSheet(
                "QTabBar {"
                "    background: transparent;"
                "    border: none;"
                "    outline: none;"
                "}"
                "QTabBar::tab {"
                "    background: transparent;"
                "    color: #999999;"
                "    padding: 8px 12px;"
                "    font-size: 13px;"
                "    font-weight: 500;"
                "    border: none;"
                "    border-bottom: 2px solid transparent;"
                "    border-top-left-radius: 6px;"
                "    border-top-right-radius: 6px;"
                "    outline: none;"
                "    margin-right: 4px;"
                "}"
                "QTabBar::tab:hover {"
                "    background-color: #252525;"
                "    color: #cccccc;"
                "}"
                "QTabBar::tab:selected {"
                "    background-color: #2b2b2b;"
                "    color: #ffffff;"
                "    font-weight: 750;"
                "    border-bottom: 2px solid #8c82ff;"
                "}"
            );

            QWidget* statsWidget = setupStats(ribbonBar);
			QWidget* quickActions = setupQuickActions(ribbonBar);

            QHBoxLayout* layout = new QHBoxLayout(ribbonBar);
            layout->setContentsMargins(30, 0, 0, 0);
            layout->setSpacing(0);
            layout->addWidget(ribbonTabs);
			layout->addWidget(quickActions);
            layout->addStretch();
            layout->addWidget(statsWidget);

			for (const auto& tabName : ribbonTabNames) {
				auto ribbonTab = ribbonTabs->addTab(QString::fromStdString(tabName));
			}

			ConnectContextMenu(ribbonBar, ribbonTabs, parent);

            return ribbonBar;
        }
    }
}