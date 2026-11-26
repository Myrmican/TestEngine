#pragma once

class MainWindow;
class QStackedWidget;
class QWidget;

namespace MainMenu {
	void initialize(MainWindow* window, QStackedWidget* workspaceStack, QWidget* editorPage);
}