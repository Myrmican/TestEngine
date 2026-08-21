#ifndef WINDOW_H
#define WINDOW_H

class QMainWindow;
class QApplication;

namespace Window {
	void initialize(QMainWindow* window, QApplication& app);
}

#endif