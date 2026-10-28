#include <project/Project.h>
#include <editor/EngineViewport.h>
#include <QWidget>
#include <QHBoxLayout>
#include <QSplitter>
#include <QTableWidget>
#include <QDockWidget>
#include <QMainWindow>
#include <QToolBar>
#include <QLabel>
#include <QTimer>
#include <QElapsedTimer>
#include <QMenu>
#include <QStackedWidget>
#include <QActionGroup>
#include <core/Logger.h>
#include <util/Languages.h>
#include <ui/docks/DockManager.h>
#include <ui/TabManager.h>
#include <editor/EditorWindow.h>
#include <ui/ribbon/Ribbon.h>
#include <ui/Toolbar.h>
#include <editor/tools/ToolManager.h>

namespace Engine {
    EditorWindow::EditorWindow(Project* project, QWidget* editorPage, QMainWindow* window) {
        if (editorPage) {
            editorPage->setObjectName("ProjectEditorTopWidget");
            editorPage->setProperty("projectInstance", QVariant::fromValue(static_cast<void*>(project)));
            project->setParent(editorPage);
        }

        window->setProperty("projectInstance", QVariant::fromValue(static_cast<void*>(project)));
        window->setWindowTitle(project->name + " - Test Engine");

        Engine::Ribbon* editorRibbon = new Engine::Ribbon(window);

        auto* editorLayout = new QVBoxLayout(editorPage);
        editorLayout->setContentsMargins(0, 0, 0, 0);
        editorLayout->setSpacing(0);

        TabManager::FileTabs* documentTabs = new TabManager::FileTabs(editorPage);
        editorLayout->addWidget(documentTabs);

        window->setDockOptions(QMainWindow::AllowNestedDocks |
                QMainWindow::AllowTabbedDocks |
                QMainWindow::GroupedDragging);

        /*window->setStyleSheet(
            "QMainWindow::separator {"
            "    background-color: #1e1e1e;"
            "}"
        );*/

        EngineViewport* placeView = new EngineViewport(editorPage, project);

        auto windowDocks = DockManager::setup(window, project);
        const int projectTabIndex = documentTabs->addTab(placeView, project->name);

        window->show();

        auto* frameTimer = new QTimer(window);
        frameTimer->setInterval(0);

        static QElapsedTimer elapsedTimer;
        elapsedTimer.start();

        QObject::connect(frameTimer, &QTimer::timeout, window, [project]() {
            static qint64 lastTime = elapsedTimer.nsecsElapsed();
            qint64 currentTime = elapsedTimer.nsecsElapsed();
            float deltaTime = (currentTime - lastTime) / 1000000000.0f;
            lastTime = currentTime;

            if (project && project->getEngine()) {
                project->getEngine()->update(deltaTime);
            }
            });

        frameTimer->start();

        QObject::connect(documentTabs, &QTabWidget::tabCloseRequested,
            [documentTabs, placeView, editorPage, window, project](int index) {
                documentTabs->handleTabClose(index, placeView, editorPage, window, project);
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
        }

        Logger* outputLogger = new Logger(windowDocks.output, project);
        project->logger = outputLogger;
    }
}