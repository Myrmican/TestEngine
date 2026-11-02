#include <ui/menus/FileMenu.h>
#include <project/Project.h>
#include <editor/EditorWindow.h>
#include <ui/ProjectLoadingDialog.h>
#include <core/settings/game/GameSettings.h>
#include <core/settings/studio/StudioSettings.h>
#include <project/saving/SaveProject.h>
#include <project/publishing/PublishProject.h>
#include <ui/menus/MenuManager.h>
#include <QKeySequence>
#include <QTimer>
#include <QApplication>

FileMenu::FileMenu(QMenuBar* menuBar, QMainWindow* window, QStackedWidget* workspaceStack, QWidget* editorPage)
    : m_workspaceStack(workspaceStack), m_editorPage(editorPage)
{
    fileMenu = Menu::create(menuBar, "File");
    menuBar->addMenu(fileMenu);

    newAction = fileMenu->addAction("New", [window, workspaceStack, editorPage]() {
        if (auto project = ProjectManager::createNew(window)) {
            auto* loadingDialog = new ProjectLoadingDialog(project->name, window);
            loadingDialog->setAttribute(Qt::WA_DeleteOnClose);
            loadingDialog->show();

            QTimer::singleShot(0, window, [window, workspaceStack, editorPage, project, loadingDialog]() {
                loadingDialog->setStatus("Building editor...");
                QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

                new Engine::EditorWindow(project, editorPage, window);

                workspaceStack->setCurrentIndex(1);
                loadingDialog->close();
                });
        }
        });
    newAction->setShortcut(QKeySequence("Ctrl+N"));
    newAction->setShortcutContext(Qt::WindowShortcut);

    openAction = fileMenu->addAction("Open", [window, workspaceStack, editorPage]() {
        if (auto project = ProjectManager::openFile(window)) {
            auto* loadingDialog = new ProjectLoadingDialog(project->name, window);
            loadingDialog->setAttribute(Qt::WA_DeleteOnClose);
            loadingDialog->show();

            QTimer::singleShot(0, window, [window, workspaceStack, editorPage, project, loadingDialog]() {
                loadingDialog->setStatus("Building editor...");
                QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

                new Engine::EditorWindow(project, editorPage, window);

                workspaceStack->setCurrentIndex(1);
                loadingDialog->close();
                });
        }
        });

    openAction->setShortcut(QKeySequence("Ctrl+O"));
    openAction->setShortcutContext(Qt::WindowShortcut);

    fileRecentMenu = Menu::create(fileMenu, "Recents");
    fileRecentMenu->setObjectName("RecentOpenProjects");
    fileMenu->addMenu(fileRecentMenu);

    fileMenu->addSeparator();

    saveAction = fileMenu->addAction("Save", [editorPage]() {
        if (auto activeProject = ProjectManager::getProject(editorPage)) {
            SaveProject::initSave(activeProject, editorPage);
        }
        });
    saveAction->setShortcut(QKeySequence("Ctrl+S"));
    saveAction->setShortcutContext(Qt::WindowShortcut);

    publishAction = fileMenu->addAction("Publish", [editorPage]() {
        if (auto activeProject = ProjectManager::getProject(editorPage)) {
            PublishProject::initPublish(activeProject);
        }
        });
    publishAction->setShortcut(QKeySequence("Alt+P"));
    publishAction->setShortcutContext(Qt::WindowShortcut);

    fileMenu->addSeparator();

    gameSettingsAction = fileMenu->addAction("Game Settings", [editorPage]() {
        GameSettings::open(editorPage);
        });

    fileMenu->addSeparator();

    studioSettingsAction = fileMenu->addAction("Studio Settings", [editorPage]() {
        StudioSettings::open(editorPage);
        });
    studioSettingsAction->setShortcut(QKeySequence("Alt+S"));
    studioSettingsAction->setShortcutContext(Qt::WindowShortcut);

    aboutAction = fileMenu->addAction("About Test Engine");
    fileMenu->addSeparator();

    exitAction = fileMenu->addAction("Exit", window, &QMainWindow::close);

    QObject::connect(fileMenu, &QMenu::aboutToShow, this, [this]() {
        fileRecentMenu->clear();

        bool isInProject = (m_workspaceStack->currentIndex() == 1);

        saveAction->setVisible(isInProject);
        publishAction->setVisible(isInProject);
        gameSettingsAction->setVisible(isInProject);
        });
}