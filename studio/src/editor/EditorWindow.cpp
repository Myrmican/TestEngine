#include <project/Project.h>
#include <QWidget>
#include <QHBoxLayout>
#include <QSplitter>
#include <QTableWidget>
#include <QTextEdit>
#include <QDockWidget>
#include <QMainWindow>
#include <QHeaderView>
#include <QToolBar>
#include <QLabel>
#include <QWidgetAction>
#include <QPushButton>
#include <QTimer>
#include <QMenu>
#include <QStackedWidget>
#include <QActionGroup>
#include <QToolButton>
#include <core/Logger.h>
#include <util/Languages.h>
#include <ui/docks/DockManager.h>
#include <ui/TabManager.h>
#include <ui/Toolbar.h>

namespace EditorWindow {
    void initialize(Project* project, QWidget* editorPage, QMainWindow* window) {

        if (editorPage) {
            editorPage->setProperty("projectInstance", QVariant::fromValue(static_cast<void*>(project)));
            project->setParent(editorPage);
        }

        QToolBar* mainToolBar = Toolbar::create(window);

        auto* centerSplitter = new QSplitter(Qt::Vertical, editorPage);

        auto* editorLayout = new QVBoxLayout(editorPage);
        editorLayout->setContentsMargins(0, 0, 0, 0);
        editorLayout->addWidget(centerSplitter);

        window->setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowNestedDocks);

        QTabWidget* documentTabs = TabManager::createWidget(centerSplitter);
        QWidget* placeView = new QWidget();

        auto windowDocks = DockManager::setup(window, project);
        const int projectTabIndex = documentTabs->addTab(placeView, project->name);

        QObject::connect(documentTabs, &QTabWidget::tabCloseRequested,
            [documentTabs, projectTabIndex, editorPage, window, mainToolBar, project](int index) {
                TabManager::handleTabClose(index, projectTabIndex, documentTabs, editorPage, window, mainToolBar, project);
            }
        );

        centerSplitter->addWidget(documentTabs);
        centerSplitter->setStretchFactor(0, 3);
        centerSplitter->setStretchFactor(1, 1);

        //Add project to Recent list

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
	}
}