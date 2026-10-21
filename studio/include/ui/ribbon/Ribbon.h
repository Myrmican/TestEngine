#pragma once

#include <QFrame>

class QMainWindow;
class QToolBar;

namespace Engine {
	class Ribbon : public QFrame {
	public:
		QToolBar* toolbar;

		Ribbon(QMainWindow* parent);
	};
}