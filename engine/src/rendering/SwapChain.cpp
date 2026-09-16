#include "rendering/SwapChain.h"
#include <dxgi1_2.h>

using namespace Engine;

bool SwapChain::Init(RenderDevice* device, HWND hwnd, int width, int height)
{
    m_device = device;
    m_width = width;
    m_height = height;

    // To make a swap chain for an EXISTING device, we have to go find the
    // DXGI factory that device was created through. This 3-step "walk up"
    // (device -> adapter -> factory) is the standard pattern for adding
    // extra windows/swap chains to a device you already made elsewhere.
    ComPtr<IDXGIDevice> dxgiDevice;
    device->getDevice()->QueryInterface(IID_PPV_ARGS(&dxgiDevice));

    ComPtr<IDXGIAdapter> adapter;
    dxgiDevice->GetAdapter(&adapter);

    ComPtr<IDXGIFactory> factory;
    adapter->GetParent(IID_PPV_ARGS(&factory));

    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferCount = 1;
    scd.BufferDesc.Width = width;
    scd.BufferDesc.Height = height;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferDesc.RefreshRate.Numerator = 60;
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = hwnd;
    scd.SampleDesc.Count = 1;
    scd.Windowed = TRUE;

    HRESULT hr = factory->CreateSwapChain(device->getDevice(), &scd, &m_swapChain);
    if (FAILED(hr))
        return false;

    CreateRenderTargetView();
    return true;
}

void SwapChain::CreateRenderTargetView()
{
    ComPtr<ID3D11Texture2D> backBuffer;
    m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
    m_device->getDevice()->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_renderTargetView);
}

void SwapChain::Resize(int width, int height)
{
    m_width = width;
    m_height = height;

    m_renderTargetView.Reset(); // must release before ResizeBuffers
    m_swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
    CreateRenderTargetView();
}

void SwapChain::BeginFrame(ID3D11DeviceContext* context, const float clearColor[4])
{
    context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), nullptr);

    D3D11_VIEWPORT vp = {};
    vp.Width = static_cast<float>(m_width);
    vp.Height = static_cast<float>(m_height);
    vp.MaxDepth = 1.0f;
    context->RSSetViewports(1, &vp);

    context->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);
}