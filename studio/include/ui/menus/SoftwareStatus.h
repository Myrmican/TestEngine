#pragma once

#include <QMenu>
#include <QMenuBar>
#include <QStackedWidget>

class SoftwareStatus : public QMenu {
	Q_OBJECT

public:
	explicit SoftwareStatus(QMenuBar* menuBar);
};