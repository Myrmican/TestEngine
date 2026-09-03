#include <ui/menus/MenuManager.h>
#include <QMenu>
#include <QFrame>
#include <QTabBar>
#include <QHBoxLayout>
#include <QLabel>
#include <QSizePolicy>
#include <QMainWindow>

void ConnectContextMenu(QWidget* ribbonBar, QTabBar* ribbonTabs, QMainWindow* window) {
    QObject::connect(ribbonBar, &QWidget::customContextMenuRequested,
        [ribbonBar, ribbonTabs, window](const QPoint& pos) {
            QPoint tabBarPos = ribbonBar->mapFrom(ribbonBar, pos);

            QMenu* contextMenu = Menu::create(ribbonBar);

            int index = ribbonTabs->tabAt(tabBarPos);

            if (index != -1) {
				QMenu* tabMenu = Menu::create(contextMenu);
                tabMenu->addAction("Rename tab");
                tabMenu->addAction("Hide tab", [ribbonTabs, index]() {
                    ribbonTabs->setTabVisible(index, false);
                });
            } else {
                QMenu* ribbonTabMenu = Menu::create(contextMenu, "Ribbon tabs");

                contextMenu->addMenu(ribbonTabMenu);

                QAction* manageRibbonTabs = ribbonTabMenu->addAction("Manage");

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
                "QTabBar::tab:first {"
                "    margin-left: 30px;"
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

            QHBoxLayout* layout = new QHBoxLayout(ribbonBar);
            layout->setContentsMargins(0, 0, 0, 0);
            layout->setSpacing(0);
            layout->addWidget(ribbonTabs);
            layout->addStretch();
            layout->addWidget(statsWidget);

			ConnectContextMenu(ribbonBar, ribbonTabs, parent);

            return ribbonBar;
        }
    }
}