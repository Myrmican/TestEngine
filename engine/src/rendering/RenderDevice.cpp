#include "rendering/RenderDevice.h"

using namespace Engine;

bool RenderDevice::Init()
{
    UINT createFlags = 0;
#ifdef _DEBUG
    createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL featureLevel;
    HRESULT hr = D3D11CreateDevice(
        nullptr,                   // default GPU adapter
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        createFlags,
        nullptr, 0,
        D3D11_SDK_VERSION,
        &m_device,
        &featureLevel,
        &m_context
    );

    return SUCCEEDED(hr);
}