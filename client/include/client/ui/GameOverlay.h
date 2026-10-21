#pragma once

#include <windows.h>
#include <string>

namespace Client {

    class GameOverlay {
    public:
        void update(float deltaTime) {
            m_frameCount++;
            m_timeAccumulator += deltaTime;

            // Recalculate FPS once every second to keep the display stable
            if (m_timeAccumulator >= 1.0f) {
                m_currentFps = static_cast<float>(m_frameCount) / m_timeAccumulator;
                m_frameCount = 0;
                m_timeAccumulator = 0.0f;
            }
        }

        void renderWin32(HWND hWnd) const {
            HDC hdc = GetDC(hWnd);
            if (!hdc) return;

            // Format the text string
            std::string text = "FPS: " + std::to_string(static_cast<int>(m_currentFps));

            // Styling: transparent background, green text
            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, RGB(0, 255, 0));

            // Draw in top-right corner
            RECT rect;
            GetClientRect(hWnd, &rect);
            rect.left = rect.right - 100;
            rect.top = 10;

            DrawTextA(hdc, text.c_str(), -1, &rect, DT_SINGLELINE | DT_LEFT);

            ReleaseDC(hWnd, hdc);
        }

    private:
        int m_frameCount = 0;
        float m_timeAccumulator = 0.0f;
        float m_currentFps = 0.0f;
    };

}