#pragma once

#include <ui/window/MainWindow.h>
#include <QMenuBar>
#include <QStackedWidget>
#include <QWidget>
#include <QMenu>
#include <QAction>

class FileMenu : public QObject {
    Q_OBJECT
public:
    FileMenu(QMenuBar* menuBar, MainWindow* window, QStackedWidget* workspaceStack, QWidget* editorPage);

private:
    void updateMenuState();

    QMenu* fileMenu = nullptr;
    QMenu* fileRecentMenu = nullptr;

	QAction* newAction = nullptr;
	QAction* openAction = nullptr;
    QAction* saveAction = nullptr;
    QAction* publishAction = nullptr;
    QAction* gameSettingsAction = nullptr;
    QAction* studioSettingsAction = nullptr;
    QAction* aboutAction = nullptr;
    QAction* exitAction = nullptr;

    QStackedWidget* m_workspaceStack;
    QWidget* m_editorPage;
};