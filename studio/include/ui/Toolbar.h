#pragma once

class QMainWindow;
class QToolBar;

namespace Engine {
	class Ribbon;
}

namespace Toolbar {
	QToolBar* create(QMainWindow* window);
}