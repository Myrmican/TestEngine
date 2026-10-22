#pragma once

#include <QFrame>

class QMainWindow;
class QToolBar;

namespace Engine {
	class Ribbon : public QFrame {
	public:
		QToolBar* toolbar;
		QToolBar* ribbonToolbar;

		Ribbon(QMainWindow* parent);
	};
}