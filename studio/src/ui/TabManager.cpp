#include <QTabWidget>
#include <QWidget>
#include <QDockWidget>
#include <QStackedWidget>
#include <QMainWindow>
#include <QTabBar>
#include <QMenu>
#include <QShortcut>
#include <project/Project.h>
#include <ui/TabManager.h>
#include <ui/ribbon/Ribbon.h>
#include <ui/menus/MenuManager.h>

namespace TabManager {
	FileTabs::FileTabs(QWidget* parent) : QTabWidget(parent) {
        QSize size = sizeHint();
        size.setHeight(45);
        setBaseSize(size);
        setTabsClosable(true);
        setMovable(true);
        setObjectName("DocumentTabs");
        tabBar()->setContextMenuPolicy(Qt::CustomContextMenu);
        setDocumentMode(true);
        setStyleSheet(
            "QTabWidget::pane {"
            "    border: none;"
            "}"
            "QTabBar {"
            "    background: transparent;"
            "}"
            "QTabBar::tab {"
            "    background-color: #1e1e1e;"
            "    color: #f2f2f2;"
            "    padding: 8px 12px;"
            "    font-size: 12px;"
            "    border: none;"
            "    border-right: 1px solid #666666;"
            "}"
            "QTabBar::tab:hover {"
            "    background-color: #2b2b2b;"
            "}"
            "QTabBar::close-button {"
            "    image: url(:/assets/icons/close.png);"
            "    width: 16px;"
            "    height: 16px;"
            "    subcontrol-origin: padding;"
            "    subcontrol-position: right center;"
            "    right: 6px;"
            "}"
            "QTabBar::close-button:hover {"
            "    background-color: #ff4d4d;"
            "    border-radius: 2px;"
            "}"
        );

        auto* closeShortcut = new QShortcut(QKeySequence("CTRL+W"), this);
        closeShortcut->setContext(Qt::WindowShortcut);

        connect(closeShortcut, &QShortcut::activated, this, [this]() {
            int currentIndex = this->currentIndex();
            if (currentIndex >= 1) {
                emit tabCloseRequested(currentIndex);
            }
            });

        connect(tabBar(), &QTabBar::customContextMenuRequested,
            this, &FileTabs::showTabContextMenu);
    }

    void FileTabs::showTabContextMenu(const QPoint& pos) {
        int index = tabBar()->tabAt(pos);
        if (index < 1) return;

        QMenu* contextMenu = Menu::create(this->parentWidget());
        QAction* closeTabAction = contextMenu->addAction("Close Tab");

        closeTabAction->setShortcut(QKeySequence("Ctrl+W"));
        closeTabAction->setShortcutContext(Qt::WindowShortcut);

        QAction* closeOtherTabsAction = contextMenu->addAction("Close Other Tabs");
        QAction* closeAllTabsAction = contextMenu->addAction("Close All Tabs");
        QAction* selectedAction = contextMenu->exec(tabBar()->mapToGlobal(pos));
        if (selectedAction == closeTabAction) {
            emit tabCloseRequested(index);
        }
        else if (selectedAction == closeOtherTabsAction) {
            for (int i = count() - 1; i >= 0; --i) {
                if (i != index && i > 0) {
                    emit tabCloseRequested(i);
                }
            }
        }
        else if (selectedAction == closeAllTabsAction) {
            for (int i = count() - 1; i >= 0; --i) {
                if (i < 1) continue;

                emit tabCloseRequested(i);
            }
        }
    }

    void FileTabs::handleTabClose(int index, int projectTabIndex, QWidget * editorPage,
        QMainWindow * window, QToolBar * mainToolBar, Project * project) {
        
        removeTab(index);

        if (projectTabIndex != index) return;

        if (!editorPage) return;

        auto* workspaceStack = qobject_cast<QStackedWidget*>(editorPage->parentWidget());
        if (workspaceStack) {
            for (auto* oldDock : window->findChildren<QDockWidget*>()) {
                window->removeDockWidget(oldDock);
                delete oldDock;
            }

            delete mainToolBar;

            workspaceStack->setCurrentIndex(0);
            if (project->projectFile) {
                project->projectFile->close();
            }

            window->setProperty("projectInstance", QVariant());
            window->setWindowTitle("Test Engine");

            delete project;
        }
	}
}