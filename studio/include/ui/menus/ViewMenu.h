#pragma once

#include <QMenu>
#include <QMenuBar>

class MainWindow;

class ViewMenu : public QMenu {
	Q_OBJECT

public:
	explicit ViewMenu(QMenuBar* menuBar, MainWindow* window);
};