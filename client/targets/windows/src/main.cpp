#include <windows.h>
#include <tchar.h>
#include <memory>
#include <engine/Engine.h>
#include <util/System.h>
#include <chrono>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

static TCHAR szWindowClass[] = _T("DesktopApp");
static TCHAR szTitle[] = _T("Test platform");
HINSTANCE hInst;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
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

    //Create client window
    HWND hWnd = CreateWindowEx(
        0, szWindowClass, szTitle,
        WS_POPUP | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT,
        NULL, NULL, hInstance, NULL
    );

    if (!hWnd) {
        DeleteObject(hBrush);
        return 1;
    }

    ShowWindow(hWnd, SW_MAXIMIZE);
    UpdateWindow(hWnd);

    auto engine = std::make_unique<Engine::EngineInstance>();

    engine->init();

    MSG msg{};
    bool isRunning = true;

    auto lastTime = std::chrono::high_resolution_clock::now();

    while (isRunning) {
        //Process all pending Windows OS events
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                isRunning = false;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (!isRunning) break;

        if (IsIconic(hWnd)) {
            WaitMessage(); //Blocks until a new Windows message arrives
            lastTime = std::chrono::high_resolution_clock::now(); //Reset delta time to avoid huge dt jumps on resume
            continue;
        }

        //Calculate the delta time and update the engine loop
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed = currentTime - lastTime;
        lastTime = currentTime;
        float dt = elapsed.count();

        if (dt > 0.25f) dt = 0.25f;

        engine->update(dt);
    }

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
    case WM_KEYDOWN:
        if (wParam == VK_F11) { //Toggle fullscreen using F11
            
        }

        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}