#pragma once

#include <QMenu>
#include <QMenuBar>

class EditMenu : public QMenu {
	Q_OBJECT

public:
	explicit EditMenu(QMenuBar* menuBar);
};