#pragma once

#include <QFrame>

class QFrame;
class QMainWindow;
class QWidget;
class QStackedWidget;

namespace Engine {
	class Ribbon : public QFrame {
	public:
		QStackedWidget* tabBar;

		Ribbon(QMainWindow* parent);
	};
}