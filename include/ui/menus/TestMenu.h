#pragma once

#include <QMenu>
#include <QMenuBar>

class TestMenu : public QMenu {
	Q_OBJECT

public:
	explicit TestMenu(QMenuBar* menuBar);
};