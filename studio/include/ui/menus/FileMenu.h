#pragma once

#include <QMenuBar>
#include <QMainWindow>
#include <QStackedWidget>
#include <QWidget>
#include <QMenu>
#include <QAction>

class FileMenu : public QObject {
    Q_OBJECT
public:
    FileMenu(QMenuBar* menuBar, QMainWindow* window, QStackedWidget* workspaceStack, QWidget* editorPage);

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