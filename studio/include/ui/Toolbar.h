#pragma once

class MainWindow;
class QToolBar;

namespace Engine {
	class Ribbon;
}

namespace Toolbar {
	QToolBar* create(MainWindow* window);
}