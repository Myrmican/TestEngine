#pragma once

#include <QMenu>
#include <QMenuBar>

class WindowMenu : public QMenu {
	Q_OBJECT

public:
	explicit WindowMenu(QMenuBar* menuBar);
};