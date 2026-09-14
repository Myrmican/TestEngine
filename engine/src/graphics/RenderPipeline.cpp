#include <graphics/RenderPipeline.h>
#include <datamodel/instances/BasePart.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

namespace {
    std::wstring ShaderPath(const char* filename) {
        std::string path = std::string(SHADERS_DIR) + "/" + filename;
        return std::wstring(path.begin(), path.end()); // ASCII-safe widen
    }
}

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

        UINT createDeviceFlags = 0;
    #if defined(_DEBUG)
            createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    #endif

        HRESULT hr = D3D11CreateDeviceAndSwapChain(
            nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags,
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

        D3D11_DEPTH_STENCIL_DESC dsDesc = {};
        dsDesc.DepthEnable = FALSE;
        dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

        D3D11_TEXTURE2D_DESC depthDesc = {};
        depthDesc.Width = width;
        depthDesc.Height = height;
        depthDesc.MipLevels = 1;
        depthDesc.ArraySize = 1;
        depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthDesc.SampleDesc.Count = 1;
        depthDesc.Usage = D3D11_USAGE_DEFAULT;
        depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

        ComPtr<ID3D11Texture2D> depthTex;
        m_device->CreateTexture2D(&depthDesc, nullptr, depthTex.GetAddressOf());
        m_device->CreateDepthStencilView(depthTex.Get(), nullptr, m_depthStencilView.GetAddressOf());

        ComPtr<ID3D11DepthStencilState> dsState;
        m_device->CreateDepthStencilState(&dsDesc, dsState.GetAddressOf());
        m_context->OMSetDepthStencilState(dsState.Get(), 0);
    }

    bool RenderPipeline::CreateShadersAndGeometry() {
        ComPtr<ID3DBlob> vsBlob, psBlob, errorBlob;

        HRESULT hr = D3DCompileFromFile(
            ShaderPath("UnlitVS.hlsl").c_str(),
            nullptr, nullptr,
            "main", "vs_5_0",
            0, 0,
            &vsBlob, &errorBlob
        );
        if (FAILED(hr)) {
            if (errorBlob) OutputDebugStringA((char*)errorBlob->GetBufferPointer());
            return false;
        }

        hr = D3DCompileFromFile(
            ShaderPath("UnlitPS.hlsl").c_str(),
            nullptr, nullptr,
            "main", "ps_5_0",
            0, 0,
            &psBlob, &errorBlob
        );
        if (FAILED(hr)) {
            if (errorBlob) OutputDebugStringA((char*)errorBlob->GetBufferPointer());
            return false;
        }

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

        Vertex cubeVertices[] = {
            { { -0.5f,  0.5f, -0.5f } }, { {  0.5f,  0.5f, -0.5f } },
            { {  0.5f,  0.5f,  0.5f } }, { { -0.5f,  0.5f,  0.5f } },
            { { -0.5f, -0.5f, -0.5f } }, { {  0.5f, -0.5f, -0.5f } },
            { {  0.5f, -0.5f,  0.5f } }, { { -0.5f, -0.5f,  0.5f } }
        };

        uint16_t cubeIndices[] = {
            3,1,0, 2,1,3, // Top
            0,5,4, 1,5,0, // Front
            3,4,7, 0,4,3, // Left
            1,6,5, 2,6,1, // Right
            2,7,6, 3,7,2, // Back
            4,6,7, 5,6,4  // Bottom
        };

        D3D11_BUFFER_DESC ibDesc = {};
        ibDesc.Usage = D3D11_USAGE_DEFAULT;
        ibDesc.ByteWidth = sizeof(cubeIndices);
        ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

        D3D11_SUBRESOURCE_DATA ibInit = { cubeIndices };
        hr = m_device->CreateBuffer(&ibDesc, &ibInit, &m_indexBuffer);
        if (FAILED(hr)) return false;

        // --- Constant buffer (per-frame view/projection matrix) ---
        D3D11_BUFFER_DESC cbDesc = {};
        cbDesc.Usage = D3D11_USAGE_DEFAULT;
        cbDesc.ByteWidth = sizeof(PerObjectBuffer);
        cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

        hr = m_device->CreateBuffer(&cbDesc, nullptr, &m_matrixConstantBuffer);
        if (FAILED(hr)) return false;

        D3D11_BUFFER_DESC vbDesc = {};
        vbDesc.Usage = D3D11_USAGE_DEFAULT;
        vbDesc.ByteWidth = sizeof(cubeVertices);
        vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA vbInit = { cubeVertices };
        hr = m_device->CreateBuffer(&vbDesc, &vbInit, &m_vertexBuffer);
        if (FAILED(hr)) return false;

        return true;
    }

    void RenderPipeline::RenderFrame(Camera* activeCamera, const std::vector<BasePart*>& parts) {
        if (!m_context || !m_renderTargetView || !activeCamera) return;

        // 1. Bind Output Merger (Render Target & Depth View)
        ID3D11RenderTargetView* rtvs[] = { m_renderTargetView.Get() };
        m_context->OMSetRenderTargets(1, rtvs, m_depthStencilView.Get());

        // 2. Clear background & depth
        const float clearColor[4] = { 0.12f, 0.06f, 0.15f, 1.0f };
        m_context->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);
        m_context->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

        // 3. Bind Shader Pipeline & Geometry Buffers
        m_context->IASetInputLayout(m_inputLayout.Get());
        m_context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
        m_context->VSSetConstantBuffers(0, 1, m_matrixConstantBuffer.GetAddressOf());
        m_context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

        UINT stride = sizeof(Vertex), offset = 0;
        m_context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
        m_context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
        m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        float aspect = static_cast<float>(m_viewportWidth) / static_cast<float>(m_viewportHeight);
        DirectX::XMMATRIX viewProj = activeCamera->GetViewProjectionMatrix(aspect);

        // 4. Draw Parts
        for (const BasePart* part : parts) {
            if (!part) continue;

            // Calculate Transposed WVP Matrix
            DirectX::XMMATRIX world = part->GetWorldMatrix();
            DirectX::XMMATRIX wvpMatrix = DirectX::XMMatrixTranspose(world * viewProj);

            PerObjectBuffer cb;
            DirectX::XMStoreFloat4x4(&cb.worldViewProjection, wvpMatrix);
            cb.color = DirectX::XMFLOAT4(0.2, 0, 1, 1.0f);

            // Send Constant Buffer update to GPU
            m_context->UpdateSubresource(m_matrixConstantBuffer.Get(), 0, nullptr, &cb, 0, 0);

            // Draw 36 indices for this part
            m_context->DrawIndexed(36, 0, 0);
        }

        m_swapChain->Present(1, 0);
    }

}