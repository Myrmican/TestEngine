
#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <wrl/client.h>
#include "rendering/RenderDevice.h"

using Microsoft::WRL::ComPtr;

namespace Engine {
    class SwapChain {
    public:
        bool Init(RenderDevice* device, HWND hwnd, int width, int height);
        void Resize(int width, int height);

        // Clears + binds this swap chain's back buffer as the render target.
        // Called at the start of every frame, before drawing anything.
        void BeginFrame(ID3D11DeviceContext* context, const float clearColor[4]);

        void Present() { m_swapChain->Present(1, 0); }

        float getAspectRatio() const {
            return m_height > 0 ? static_cast<float>(m_width) / m_height : 1.0f;
        }

    private:
        void CreateRenderTargetView();

        RenderDevice* m_device = nullptr; // not owned
        ComPtr<IDXGISwapChain> m_swapChain;
        ComPtr<ID3D11RenderTargetView> m_renderTargetView;
        int m_width = 0, m_height = 0;
    };
}