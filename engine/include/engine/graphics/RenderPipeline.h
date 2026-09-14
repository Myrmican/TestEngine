#pragma once

#include <d3d11.h>
#include <dxgi.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include <datamodel/instances/Camera.h>
#include <datamodel/instances/BasePart.h>

struct alignas(16) PerObjectBuffer {
    DirectX::XMFLOAT4X4 worldViewProjection; // Changed from XMMATRIX to XMFLOAT4X4!
    DirectX::XMFLOAT4 color;
};

namespace Engine {

    template <typename T>
    using ComPtr = Microsoft::WRL::ComPtr<T>;

    // Matches the matrix constant buffer in UnlitVS.hlsl
    struct MatrixBuffer {
        DirectX::XMMATRIX viewProjection;
    };

    // Vertex format layout
    struct Vertex {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT4 color;
    };

    class RenderPipeline {
    public:
        RenderPipeline() = default;
        ~RenderPipeline() = default;

        bool Initialize(HWND windowHandle, int width, int height);
        void Resize(int width, int height);
        void RenderFrame(Camera* activeCamera, const std::vector<BasePart*>& parts);

    private:
        bool CreateShadersAndGeometry();

        Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;

        // DirectX 11 Core Interfaces
        ComPtr<ID3D11Device> m_device;
        ComPtr<ID3D11DeviceContext> m_context;
        ComPtr<IDXGISwapChain> m_swapChain;
        ComPtr<ID3D11RenderTargetView> m_renderTargetView;

        // Pipeline Resources
        ComPtr<ID3D11Buffer> m_matrixConstantBuffer;
        ComPtr<ID3D11Buffer> m_vertexBuffer;
        ComPtr<ID3D11VertexShader> m_vertexShader;
        ComPtr<ID3D11PixelShader> m_pixelShader;
        ComPtr<ID3D11InputLayout> m_inputLayout;

        int m_viewportWidth = 800;
        int m_viewportHeight = 600;
    };

}