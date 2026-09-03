#include <QTabWidget>
#include <QWidget>
#include <QDockWidget>
#include <QStackedWidget>
#include <QMainWindow>
#include <QTabBar>
#include <project/Project.h>
#include <ui/TabManager.h>
#include <ui/Ribbon.h>

QTabWidget* TabManager::createWidget(QWidget* parent) {

    QTabWidget* tabsWidget = new QTabWidget(parent);
    QSize size = tabsWidget->sizeHint();
    size.setHeight(45);
    tabsWidget->setBaseSize(size);
    tabsWidget->setTabsClosable(true);
    tabsWidget->setMovable(true);
    tabsWidget->setObjectName("DocumentTabs");
    tabsWidget->setDocumentMode(true);
    tabsWidget->setStyleSheet(
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

    return tabsWidget;
}

void TabManager::handleTabClose(int index, int projectTabIndex, QTabWidget* tabsWidget, QWidget* editorPage,
    QMainWindow* window, QToolBar* mainToolBar, Project* project) {

    QWidget* w = tabsWidget->widget(index);
    tabsWidget->removeTab(index);
    delete w;

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

        window->setWindowTitle("Test Engine");

        delete project;
    }
}