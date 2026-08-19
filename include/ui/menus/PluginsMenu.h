#pragma once

#include <QMenu>
#include <QMenuBar>
#include <QStackedWidget>

class PluginsMenu : public QMenu {
	Q_OBJECT

public:
	explicit PluginsMenu(QMenuBar* menuBar);
};