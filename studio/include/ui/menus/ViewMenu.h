#pragma once

#include <QMenu>
#include <QMenuBar>
#include <QMainWindow>

class ViewMenu : public QMenu {
	Q_OBJECT

public:
	explicit ViewMenu(QMenuBar* menuBar, QMainWindow* window);
};