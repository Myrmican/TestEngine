#include <windows.h>
#include <tchar.h>
#include <memory>
#include <engine/Engine.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

static TCHAR szWindowClass[] = _T("DesktopApp");
static TCHAR szTitle[] = _T("Test platform");
HINSTANCE hInst;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
    // 1. Create dark/grey GDI background brush
    HBRUSH hBrush = CreateSolidBrush(RGB(30, 30, 30));

    WNDCLASSEX wcex{};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0; // Set to 0 since we store data elsewhere
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = hBrush;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex)) {
        MessageBox(NULL, _T("Call to RegisterClassEx failed!"), _T("Error"), MB_OK | MB_ICONERROR);
        DeleteObject(hBrush);
        return 1;
    }

    hInst = hInstance;

    // 2. Create the Window
    HWND hWnd = CreateWindowEx(
        0, szWindowClass, szTitle,
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT,
        NULL, NULL, hInstance, NULL
    );

    if (!hWnd) {
        DeleteObject(hBrush);
        return 1;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // 3. Initialize Engine Instance
    auto engine = std::make_unique<Engine::EngineInstance>();
    if (!engine) {
        DeleteObject(hBrush);
        return 1;
    }

    // 4. Non-Blocking Game Loop (PeekMessage)
    MSG msg{};
    bool isRunning = true;

    while (isRunning) {
        // Process all pending Windows OS events
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                isRunning = false;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (!isRunning) break;

        // --- GAME ENGINE FRAME TICK ---
        // engine->tick(deltaTime);
        // engine->render(hWnd);
    }

    // 5. Cleanup OS resources
    DeleteObject(hBrush);

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}