#pragma once

#include <QMenu>
#include <QStackedWidget>

class QMainWindow;
class QWidget;
class QMenuBar;

class FileMenu : public QMenu {
	Q_OBJECT

public:
	explicit FileMenu(QMenuBar* menuBar, QMainWindow* window, QStackedWidget* workspaceStack, QWidget* editorPage);
};