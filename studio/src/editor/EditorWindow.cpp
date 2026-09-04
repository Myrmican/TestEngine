#include <project/Project.h>
#include <QWidget>
#include <QHBoxLayout>
#include <QSplitter>
#include <QTableWidget>
#include <QDockWidget>
#include <QMainWindow>
#include <QToolBar>
#include <QLabel>
#include <QTimer>
#include <QMenu>
#include <QStackedWidget>
#include <QActionGroup>
#include <core/Logger.h>
#include <util/Languages.h>
#include <ui/docks/DockManager.h>
#include <ui/TabManager.h>
#include <ui/Ribbon.h>
#include <ui/Toolbar.h>
#include <editor/TestView.h>

namespace EditorWindow {
    void initialize(Project* project, QWidget* editorPage, QMainWindow* window) {

        if (editorPage) {
            editorPage->setProperty("projectInstance", QVariant::fromValue(static_cast<void*>(project)));
            project->setParent(editorPage);
        }

        window->setWindowTitle(project->name + " - Test Engine");

        auto ribbonBar = Engine::Ribbon::createWidget(window);

        QToolBar* ribbonToolBar = new QToolBar("RibbonToolBar", window);
        ribbonToolBar->setMovable(false);
        ribbonToolBar->setFloatable(false);
        ribbonToolBar->toggleViewAction()->setVisible(false);
        ribbonToolBar->addWidget(ribbonBar);

        QToolBar* mainToolBar = Toolbar::create(window);

        window->addToolBar(Qt::TopToolBarArea, ribbonToolBar);
        window->addToolBarBreak(Qt::TopToolBarArea);
        window->addToolBar(Qt::TopToolBarArea, mainToolBar);

        auto* editorLayout = new QVBoxLayout(editorPage);
        editorLayout->setContentsMargins(0, 0, 0, 0);
        editorLayout->setSpacing(0);

        QTabWidget* documentTabs = TabManager::createWidget(editorPage);
        editorLayout->addWidget(documentTabs);

        window->setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowNestedDocks);

        QWidget* placeView = new EngineRenderView();

        auto windowDocks = DockManager::setup(window, project);
        const int projectTabIndex = documentTabs->addTab(placeView, project->name);

        QObject::connect(documentTabs, &QTabWidget::tabCloseRequested,
            [documentTabs, projectTabIndex, editorPage, window, mainToolBar, project](int index) {
                TabManager::handleTabClose(index, projectTabIndex, documentTabs, editorPage, window, mainToolBar, project);
            }
        );

        // Recent menu logic
        QWidget* topLevelWindow = window->window();
        auto recentMenus = topLevelWindow->findChildren<QMenu*>("RecentOpenProjects");

        for (QMenu* fileRecentMenu : recentMenus) {
            QAction* projectInRecents = fileRecentMenu->findChild<QAction*>("RecentProject_" + project->name);
            if (projectInRecents) {
                delete projectInRecents;
            }

            QAction* recentAction = fileRecentMenu->addAction(project->name, [project, editorPage, window]() {
                EditorWindow::initialize(project, editorPage, window);
                });
            recentAction->setObjectName("RecentProject_" + project->name);
        }

        Logger* outputLogger = new Logger(windowDocks.output);
        project->logger = outputLogger;
        project->explorer = windowDocks.explorer;
    }
}