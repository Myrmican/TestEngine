#pragma once

#include <d3d11.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

namespace Engine {
    class RenderDevice {
    public:
        bool Init();

        ID3D11Device* getDevice() const { return m_device.Get(); }
        ID3D11DeviceContext* getContext() const { return m_context.Get(); }

    private:
        ComPtr<ID3D11Device> m_device;
        ComPtr<ID3D11DeviceContext> m_context;
    };
}