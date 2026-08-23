#include <ui/menus/FileMenu.h>
#include <QApplication>
#include <project/Project.h>
#include <project/EditorWindow.h>
#include <ui/ProjectLoadingDialog.h>
#include <core/settings/game/GameSettings.h>
#include <core/settings/studio/StudioSettings.h>
#include <project/saving/SaveProject.h>
#include <project/publishing/PublishProject.h>
#include <ui/menus/MenuManager.h>
#include <QMainWindow>
#include <QMenuBar>
#include <QTimer>
#include <QPalette>
#include <QStyleFactory>

FileMenu::FileMenu(QMenuBar* menuBar, QMainWindow* window, QStackedWidget* workspaceStack, QWidget* editorPage) {
    QMenu* fileMenu = Menu::create(menuBar, "File");
    menuBar->addMenu(fileMenu);

    QObject::connect(fileMenu, &QMenu::aboutToShow, fileMenu, [fileMenu, window, workspaceStack, editorPage, menuBar]() {
        fileMenu->clear();

        fileMenu->addAction("New", [window, workspaceStack, editorPage]() {
            if (Project* project = ProjectManager::onNewProject(window)) {
                auto* loadingDialog = new ProjectLoadingDialog(project->name, window);
                loadingDialog->setAttribute(Qt::WA_DeleteOnClose);
                loadingDialog->show();

                QTimer::singleShot(0, window, [window, workspaceStack, editorPage, project, loadingDialog]() {
                    loadingDialog->setStatus("Building editor...");
                    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

                    EditorWindow::initialize(project, editorPage, window);

                    workspaceStack->setCurrentIndex(1);
                    loadingDialog->close();
                    });
            }
            });

        fileMenu->addAction("Open", [window]() { ProjectManager::onOpenProject(window); });

        QMenu* fileRecentMenu = Menu::create(fileMenu, "Recents");
        fileMenu->addMenu(fileRecentMenu);
        fileRecentMenu->setObjectName("RecentOpenProjects");

        fileMenu->addSeparator();

        bool isInProject = (workspaceStack->currentIndex() == 1);

        if (isInProject) {
            QAction* saveAction = fileMenu->addAction("Save", [editorPage]() {
                Project* activeProject = ProjectManager::getProject(editorPage);

                SaveProject::initSave(activeProject, editorPage);
                });
            saveAction->setShortcut(QKeySequence("Ctrl+S"));
            saveAction->setShortcutContext(Qt::WindowShortcut);

            //fileMenu->addSeparator();

            QAction* publishAction = fileMenu->addAction("Publish", [editorPage]() {
                Project* activeProject = ProjectManager::getProject(editorPage);

                PublishProject::initPublish();
                });
            publishAction->setShortcut(QKeySequence("Alt+P"));
            publishAction->setShortcutContext(Qt::WindowShortcut);

            fileMenu->addSeparator();

            fileMenu->addAction("Download a copy");

            fileMenu->addSeparator();

            fileMenu->addAction("Game Settings", [editorPage]() {
                GameSettings::open(editorPage);
                });
            fileMenu->addSeparator();
        }

        QAction* studioSettingsAction = fileMenu->addAction("Studio Settings", [editorPage]() {
            StudioSettings::open(editorPage);
            });

        studioSettingsAction->setShortcut(QKeySequence("Alt+S"));
        studioSettingsAction->setShortcutContext(Qt::WindowShortcut);

        fileMenu->addAction("About Test Engine");
        fileMenu->addSeparator();
        fileMenu->addAction("Exit", window, &QMainWindow::close);
        });
}