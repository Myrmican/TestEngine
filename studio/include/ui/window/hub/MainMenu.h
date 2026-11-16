#pragma once

class QMainWindow;
class QStackedWidget;
class QWidget;

namespace MainMenu {
	void initialize(QMainWindow* window, QStackedWidget* workspaceStack, QWidget* editorPage);
}