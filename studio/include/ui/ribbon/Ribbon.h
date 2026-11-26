#pragma once

#include <QFrame>

class MainWindow;
class QToolBar;

namespace Engine {
	class Ribbon : public QFrame {
	public:
		QToolBar* toolbar;
		QToolBar* ribbonToolbar;

		Ribbon(MainWindow* parent);
	};
}