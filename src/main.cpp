#include <Window.h>
#include "Environment.h"
#include <windows.h>
#include "Velopack.h"
#include <QApplication>
#include <QMainWindow>

int main(int argc, char *argv[]) {
	vpkc_app_run(nullptr);

	if (System::IsVirtualEnvironment()) {
		int errorBoxResult = MessageBox(
			NULL,
			L"This program can not run in a virtual environment.",
			L"System Error",
			MB_ICONERROR | MB_OK
		);

		return 1;
	}

	HRESULT comResult = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	if (FAILED(comResult)) {
		return 2;
	}

	qputenv("QWK_CORE_SHARE_CONTEXT", "1");

    QApplication app(argc, argv);

    auto* window = new QMainWindow();
    window->resize(1024, 768);
    window->setWindowTitle("Test Engine");

	window->winId();

	Window::initialize(window, app);

    window->showMaximized();

	app.exec();

	CoUninitialize();

	return 0;
}