#include <graphics/RenderPipeline.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

namespace Engine {

    bool RenderPipeline::Initialize(HWND windowHandle, int width, int height) {
        m_viewportWidth = width;
        m_viewportHeight = height;

        // 1. Describe and create the SwapChain
        DXGI_SWAP_CHAIN_DESC sd = {};
        sd.BufferCount = 1;
        sd.BufferDesc.Width = width;
        sd.BufferDesc.Height = height;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = windowHandle;
        sd.SampleDesc.Count = 1;
        sd.Windowed = TRUE;
        // SwapEffect defaults to DXGI_SWAP_EFFECT_DISCARD (0), which is the
        // correct/safe choice for a plain child HWND like a Qt-embedded view.
        // (Flip-model swap effects are intended for top-level/DirectComposition
        // surfaces and can fail to composite correctly inside a child window.)

        HRESULT hr = D3D11CreateDeviceAndSwapChain(
            nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
            nullptr, 0, D3D11_SDK_VERSION, &sd,
            m_swapChain.GetAddressOf(), m_device.GetAddressOf(), nullptr, m_context.GetAddressOf()
        );

        if (FAILED(hr)) return false;

        Resize(width, height);
        return CreateShadersAndGeometry();
    }

    void RenderPipeline::Resize(int width, int height) {
        if (!m_swapChain) return;

        m_viewportWidth = width;
        m_viewportHeight = height;

        // Release old render target before resizing backbuffer
        m_renderTargetView.Reset();
        m_swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);

        ComPtr<ID3D11Texture2D> backBuffer;
        m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
        m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.GetAddressOf());

        D3D11_VIEWPORT vp = {};
        vp.Width = static_cast<float>(width);
        vp.Height = static_cast<float>(height);
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        m_context->RSSetViewports(1, &vp);
    }

    // RenderPipeline.cpp

    const char* vsSource = R"(
cbuffer MatrixBuffer : register(b0) { matrix gViewProjection; };
struct VSInput { float3 position : POSITION; float4 color : COLOR; };
struct PSInput { float4 position : SV_POSITION; float4 color : COLOR; };
PSInput main(VSInput input) {
    PSInput output;
    output.position = mul(float4(input.position, 1.0f), gViewProjection);
    output.color = input.color;
    return output;
}
)";

    const char* psSource = R"(
struct PSInput { float4 position : SV_POSITION; float4 color : COLOR; };
float4 main(PSInput input) : SV_TARGET { return input.color; }
)";

    bool RenderPipeline::CreateShadersAndGeometry() {
        ComPtr<ID3DBlob> vsBlob, psBlob, errorBlob;

        // Compile Vertex Shader from string in memory
        HRESULT hr = D3DCompile(vsSource, strlen(vsSource), nullptr, nullptr, nullptr, "main", "vs_5_0", 0, 0, &vsBlob, &errorBlob);
        if (FAILED(hr)) return false;

        // Compile Pixel Shader from string in memory
        hr = D3DCompile(psSource, strlen(psSource), nullptr, nullptr, nullptr, "main", "ps_5_0", 0, 0, &psBlob, &errorBlob);
        if (FAILED(hr)) return false;

        D3D11_RASTERIZER_DESC rasterDesc = {};
        rasterDesc.FillMode = D3D11_FILL_SOLID;
        rasterDesc.CullMode = D3D11_CULL_NONE; // Disable culling for testing
        rasterDesc.FrontCounterClockwise = FALSE;

        ComPtr<ID3D11RasterizerState> rasterState;
        hr = m_device->CreateRasterizerState(&rasterDesc, rasterState.GetAddressOf());
        if (FAILED(hr)) return false;
        m_context->RSSetState(rasterState.Get());

        hr = m_device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &m_vertexShader);
        if (FAILED(hr)) return false;

        hr = m_device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &m_pixelShader);
        if (FAILED(hr)) return false;

        // --- Input layout ---
        // Must match VSInput above (POSITION: float3, COLOR: float4) AND the
        // actual layout of your Vertex struct (see RenderPipeline.h). Adjust
        // the format/offset here if your Vertex struct's fields differ.
        D3D11_INPUT_ELEMENT_DESC layoutDesc[] = {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,                              D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,    D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        hr = m_device->CreateInputLayout(
            layoutDesc, ARRAYSIZE(layoutDesc),
            vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(),
            &m_inputLayout
        );
        if (FAILED(hr)) return false;

        // --- Vertex buffer ---
        // Simple hardcoded triangle so there is something visible to confirm
        // the pipeline works end-to-end. Replace with real scene geometry
        // once basic rendering is confirmed working.
        struct SimpleVertex {
            float position[3];
            float color[4];
        };

        SimpleVertex triangleVerts[] = {
            { {  0.0f,  0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
            { {  0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
            { { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
        };

        D3D11_BUFFER_DESC vbDesc = {};
        vbDesc.Usage = D3D11_USAGE_DEFAULT;
        vbDesc.ByteWidth = sizeof(triangleVerts);
        vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA vbInitData = {};
        vbInitData.pSysMem = triangleVerts;

        hr = m_device->CreateBuffer(&vbDesc, &vbInitData, &m_vertexBuffer);
        if (FAILED(hr)) return false;

        // --- Constant buffer (per-frame view/projection matrix) ---
        D3D11_BUFFER_DESC cbDesc = {};
        cbDesc.Usage = D3D11_USAGE_DEFAULT;
        cbDesc.ByteWidth = sizeof(MatrixBuffer);
        cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

        hr = m_device->CreateBuffer(&cbDesc, nullptr, &m_matrixConstantBuffer);
        if (FAILED(hr)) return false;

        return true;
    }

    void RenderPipeline::RenderFrame(Camera* activeCamera) {
        if (!m_context || !m_renderTargetView || !m_inputLayout || !activeCamera) {
            return;
        }

        // Clear screen background
        const float clearColor[4] = { 0.12f, 0.12f, 0.12f, 1.0f };
        m_context->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);

        // Calculate and transpose Camera matrix for HLSL
        float aspect = static_cast<float>(m_viewportWidth) / static_cast<float>(m_viewportHeight);
        MatrixBuffer cb;
        cb.viewProjection = DirectX::XMMatrixTranspose(activeCamera->GetViewProjectionMatrix(aspect));
        m_context->UpdateSubresource(m_matrixConstantBuffer.Get(), 0, nullptr, &cb, 0, 0);

        // Bind pipeline stages
        m_context->IASetInputLayout(m_inputLayout.Get());
        UINT stride = sizeof(Vertex);
        UINT offset = 0;
        m_context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
        m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        m_context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
        m_context->VSSetConstantBuffers(0, 1, m_matrixConstantBuffer.GetAddressOf());
        m_context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

        m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), nullptr);

        // Issue draw call
        m_context->Draw(3, 0);

        // Swap backbuffer to screen
        m_swapChain->Present(1, 0);
    }

}