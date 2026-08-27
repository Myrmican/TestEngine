#include <Window.h>
#include "Environment.h"
#include <windows.h>
#include "Velopack.h"
#include <QApplication>
#include <QMainWindow>
#include <engine/Engine.h>

int main(int argc, char *argv[]) {
	vpkc_app_run(nullptr);

	if (System::IsVirtualEnvironment())
		return MessageBox(
			NULL,
			L"This program can not run in a virtual environment.",
			L"System Error",
			MB_ICONERROR | MB_OK
		), 1;

	HRESULT comResult = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	if (FAILED(comResult)) {
		return 2;
	}

	qputenv("QWK_CORE_SHARE_CONTEXT", "1");

    QApplication app(argc, argv);

	Engine::initialize();

    QMainWindow window;
    window.resize(1024, 768);
    window.setWindowTitle("Test Engine");
	window.winId();

	Window::initialize(&window, app);
    window.showMaximized();

	app.exec();

	Engine::shutdown();

	CoUninitialize();

	return 0;
}