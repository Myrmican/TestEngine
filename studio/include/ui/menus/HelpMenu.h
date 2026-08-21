#pragma once

#include <QMenu>
#include <QMenuBar>
#include <QStackedWidget>

class HelpMenu : public QMenu {
	Q_OBJECT

public:
	explicit HelpMenu(QMenuBar* menuBar);
};